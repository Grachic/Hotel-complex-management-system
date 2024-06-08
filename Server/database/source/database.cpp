//
// Created by User on 16.05.2024.
//

#include "../header/database.h"

Database::Database(QObject *parent) : QObject(parent), m_databaseDirectory(QDir::currentPath()),
                                      m_databaseLogger("server") {
    m_databaseDirectory.mkpath("database/output");
}

Database::~Database() = default;

void Database::connectToDataBase() {
    if (!QFile(m_databaseDirectory.path() + "/database/output/" DATABASE_NAME).exists()) {
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

bool Database::openDataBase() {
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setHostName(DATABASE_HOSTNAME);
    m_database.setDatabaseName(m_databaseDirectory.path() + "/database/output/" DATABASE_NAME);
    if (m_database.open()) {
        return true;
    } else {
        return false;
    }
}

bool Database::restoreDataBase() {
    // Если база данных открылась ...
    if (this->openDataBase()) {
        // Производим восстановление базы данных
        bool isAllTablesAreRestored = true;
        QScopedPointer<QSqlQueryModel> model;
        for (auto &tableName: QString(ALL_DATABASE_TABLES_NAMES).simplified().split(", ")) {
            if (tableName == USER_MODEL_DATABASE_TABLE_NAME) model.reset(new UserModel(this));
            if (tableName == EMPLOYEE_MODEL_DATABASE_TABLE_NAME) model.reset(new EmployeeModel(this));
            if (tableName == HOTELROOMS_MODEL_DATABASE_TABLE_NAME) model.reset(new HotelRoomModel(this));
            if (tableName == BOOKEDDATE_MODEL_DATABASE_TABLE_NAME) model.reset(new BookedDateModel(this));
            if (!Database::createTable(tableName, model.operator*())) {
                isAllTablesAreRestored = false;
                break;
            }
        }
        return isAllTablesAreRestored;
    } else {
        qWarning(logWarning()) << "Не удалось восстановить базу данных";
        return false;
    }
}

void Database::closeDataBase() {
    m_database.close();
}

bool Database::createTable(const QString &tableName, const QSqlQueryModel &model) {
    QString queryParameters;
    for (auto &role: model.roleNames()) {
        if (!role.contains("_")) continue;
        QString tempParameter = role.split('_').first();
        if (role.contains("_txt")) tempParameter.append(" TEXT");
        if (role.contains("_int")) tempParameter.append(" INTEGER");
        if (role.contains("_rl")) tempParameter.append(" REAL");
        if (role.contains("_date")) tempParameter.append("  DATE");
        if (role.contains("_pk")) tempParameter.append(" PRIMARY KEY");
        if (role.contains("_ai")) tempParameter.append(" AUTOINCREMENT");
        if (role.contains("_nn")) tempParameter.append(" NOT NULL");
        if (role.contains("_uq")) tempParameter.append("  UNIQUE");
        tempParameter.append(",");
        queryParameters.append(tempParameter);
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE " + tableName + " (" + queryParameters.chopped(1) + " );")) {
        qCritical(logCritical()) << "DataBase: error of create " << tableName;
        qDebug(logDebug()) << query.lastError().text();
        return false;
    } else {
        if (tableName == HOTELROOMS_MODEL_DATABASE_TABLE_NAME) {
            for (int i = 1; i < HOTELROOMS_DATABASE_COUNT_FLOORS + 1; ++i) {
                for (int j = 1; j < HOTELROOMS_DATABASE_COUNT_ON_ONE_FLOOR + 1; ++j) {
                    insertIntoTable({QString::number(i) + "_" + QString::number(j)},
                                    HOTELROOMS_MODEL_DATABASE_TABLE_NAME);
                }
            }
        }
        return true;
    }
}

QSqlRecord Database::findInTable(const QVariantList &data, const QString &tableName) {
    QSqlQuery query;

    if (tableName == USER_MODEL_DATABASE_TABLE_NAME) {
        query.prepare("SELECT * FROM " USER_MODEL_DATABASE_TABLE_NAME);
    }

    if (tableName == BOOKEDDATE_MODEL_DATABASE_TABLE_NAME && data.first() == "getReserveForUser") {
        query.prepare("SELECT * FROM "
                      BOOKEDDATE_MODEL_DATABASE_TABLE_NAME " "
                      "WHERE UserLogin = :login");

        query.bindValue(":login", data.last().toString());
    }

    if (tableName == BOOKEDDATE_MODEL_DATABASE_TABLE_NAME && data.first() == "getReserveMap") {
        query.prepare("SELECT * FROM "
                      BOOKEDDATE_MODEL_DATABASE_TABLE_NAME " "
                      "WHERE (BookedDateStart >= :startDate AND BookedDateStart <= :endDate) OR "
                      "(BookedDateEnd >= :startDate AND BookedDateEnd <= :endDate)");

        query.bindValue(":startDate", data.last().toString().split("-").first().remove('.'));
        query.bindValue(":endDate", data.last().toString().split("-").last().remove('.'));
    }

    if (!query.exec()) {
        qCritical(logCritical()) << "Error find user in" << tableName;
        qDebug(logDebug()) << query.lastError().text();
        return {};
    } else {
        if (tableName == USER_MODEL_DATABASE_TABLE_NAME) {
            int loginIndex = query.record().indexOf("Login");
            while (query.next()) {
                if (query.value(loginIndex).toString() == data[0].toString()) {
                    return query.record();
                }
            }
        }

        if (tableName == BOOKEDDATE_MODEL_DATABASE_TABLE_NAME) {
            QStringList result;
            while (query.next()) {
                result.append(query.value("HotelRoomNumber").toString());
                result.append(",");
                result.append(query.value("BookedDateStart").toString());
                result.append("-");
                result.append(query.value("BookedDateEnd").toString());
            }
            QSqlRecord resultedRecord;
            for (int i = 0; i < result.count(); ++i)
                resultedRecord.setValue(i, result.at(i));
            return resultedRecord;
        }
    }

    return {};
}

bool Database::insertIntoTable(const QVariantList &data, const QString &tableName) {
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    if (tableName == USER_MODEL_DATABASE_TABLE_NAME) {
        query.prepare("INSERT INTO " USER_MODEL_DATABASE_TABLE_NAME " (" USER_MODEL_DATABASE_PARAMETERS ") "
                      "VALUES (" USER_MODEL_DATABASE_PARAMETERS_KEYS ")");
        int i = 0;
        for (auto &key: QString(USER_MODEL_DATABASE_PARAMETERS_KEYS).simplified().split(", ")) {
            qInfo(logInfo()) << key << data[i].toString();
            if (data[i].canConvert<QString>())
                query.bindValue(key, data[i].toString());
            i++;
        }
    }
    if (tableName == EMPLOYEE_MODEL_DATABASE_TABLE_NAME) {
        query.prepare("INSERT INTO " + tableName + " ( " EMPLOYEE_MODEL_DATABASE_PARAMETERS " ) "
                                                   "VALUES (" EMPLOYEE_MODEL_DATABASE_PARAMETERS_KEYS ")");
        int i = 0;
        for (auto &key: QString(EMPLOYEE_MODEL_DATABASE_PARAMETERS_KEYS).simplified().split(", ")) {
            if (data[i].canConvert<QString>())
                query.bindValue(key, data[i].toString());
            else if (data[i].canConvert<double>())
                query.bindValue(key, data[i].toDouble());
            i++;
        }
    }

    if (tableName == HOTELROOMS_MODEL_DATABASE_TABLE_NAME) {
        query.prepare("INSERT INTO " + tableName + " ( " HOTELROOMS_MODEL_DATABASE_PARAMETERS " ) "
                                                   "VALUES (" HOTELROOMS_MODEL_DATABASE_PARAMETERS_KEYS ")");
        int i = 0;
        for (auto &key: QString(HOTELROOMS_MODEL_DATABASE_PARAMETERS_KEYS).simplified().split(", ")) {
            if (data[i].canConvert<QString>())
                query.bindValue(key, data[i].toString());
            i++;
        }
    }

    if (tableName == BOOKEDDATE_MODEL_DATABASE_TABLE_NAME) {
        query.prepare("INSERT INTO " + tableName + " ( " BOOKEDDATE_MODEL_DATABASE_PARAMETERS " ) "
                                                   "VALUES (" BOOKEDDATE_MODEL_DATABASE_PARAMETERS_KEYS ")");
        int i = 0;
        for (auto &key: QString(BOOKEDDATE_MODEL_DATABASE_PARAMETERS_KEYS).simplified().split(", ")) {
            if (data[i].canConvert<QDate>())
                query.bindValue(key, data[i].toDate());
            else if (data[i].canConvert<QString>())
                query.bindValue(key, data[i].toString());
            i++;
        }
    }

    // После чего выполняется запросом методом exec()
    if (!query.exec()) {
        qCritical(logCritical()) << "Error insert into " << tableName;
        qDebug(logDebug()) << query.lastError().text();
        return false;
    } else {
        return true;
    }
}

bool Database::removeRecord(const int id, const QString &tableName) {
    // Удаление строки из базы данных будет производитсья с помощью SQL-запроса
    QSqlQuery query;

    // Удаление производим по id записи, который передается в качестве аргумента функции
    query.prepare("DELETE FROM " + tableName + " WHERE id= :ID ;");
    query.bindValue(":ID", id);

    // Выполняем удаление
    if (!query.exec()) {
        qCritical(logCritical()) << "Error delete row " << tableName;
        qDebug(logDebug()) << query.lastError().text();
        return false;
    } else {
        return true;
    }
}