//
// Created by User on 16.05.2024.
//

#include "../header/usermodel.h"

UserModel::UserModel(QObject *parent) : QSqlQueryModel(parent) {
    this->updateModel();
}

QVariant UserModel::data(const QModelIndex &index, int role) const {
    // Определяем номер колонки, адрес так сказать, по номеру роли
    int columnId = role - Qt::UserRole - 1;
    // Создаём индекс с помощью новоиспечённого ID колонки
    QModelIndex modelIndex = this->index(index.row(), columnId);

    /* И с помощью уже метода data() базового класса
     * вытаскиваем данные для таблицы из модели
     * */
    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

void UserModel::updateModel(const QString& query) {
    if (!query.isEmpty()) { this->setQuery(query); return; }
    // Обновление производится SQL-запросом к базе данных
    this->setQuery("SELECT id, " USER_MODEL_DATABASE_PARAMETERS " FROM " USER_MODEL_DATABASE_TABLE_NAME);
}

int UserModel::getId(int row) {
    return this->data(this->index(row, 0), IdRole).toInt();
}

QHash<int, QByteArray> UserModel::roleNames() const {
    /* То есть сохраняем в хеш-таблицу названия ролей
     * по их номеру
     * */
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id_int_pk_ai";
    roles[FirstNameRole] = "FirstName_txt_nn";
    roles[SecondNameRole] = "SecondName_txt";
    roles[PatronymicRole] = "Patronymic_txt";
    roles[LoginRole] = "Login_txt_nn_uq";
    roles[PasswordRole] = "Password_txt_nn";
    roles[PasswordHintRole] = "PasswordHint_txt";
    roles[PasswordRestoreRole] = "PasswordRestore_txt";
    roles[UserRole] = "UserRole_txt_nn";
    return roles;
}
