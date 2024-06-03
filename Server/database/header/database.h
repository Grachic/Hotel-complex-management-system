//
// Created by User on 16.05.2024.
//

#ifndef SERVER_DATABASE_H
#define SERVER_DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlError>
#include "../../log/header/logger.h"
#include "../../constants/constants.h"
#include "../model/header/usermodel.h"
#include "../model/header/employeemodel.h"
#include "../model/header/hotelroommodel.h"
#include "../model/header/bookeddatemodel.h"

class Database : public QObject {
Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database() override;
    void connectToDataBase();


public slots:
    static QSqlRecord findInTable(const QVariantList &data, const QString& tableName);
    static bool insertIntoTable(const QVariantList &data, const QString& tableName);      // Добавление записей в таблицу
    static bool removeRecord(int id, const QString& tableName); // Удаление записи из таблицы по её id

private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase m_database;
    QDir m_databaseDirectory;
    Logger m_databaseLogger;

    bool openDataBase();        // Открытие базы данных
    bool restoreDataBase();     // Восстановление базы данных
    void closeDataBase();       // Закрытие базы данных
    static bool createTable(const QString& tableName, const QSqlQueryModel& model);         // Создание базы таблицы в базе данных
};


#endif //SERVER_DATABASE_H
