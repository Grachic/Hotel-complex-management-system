//
// Created by User on 25.05.2024.
//

#include "../header/bookeddatemodel.h"

BookedDateModel::BookedDateModel(QObject *parent) : QSqlQueryModel(parent) {
    this->updateModel();
}

QVariant BookedDateModel::data(const QModelIndex &index, int role) const {
    int columnId = role - Qt::UserRole - 1;
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

void BookedDateModel::updateModel(const QString& query) {
    if (!query.isEmpty()) { this->setQuery(query); return; }
    this->setQuery("SELECT id, " BOOKEDDATE_MODEL_DATABASE_PARAMETERS " FROM " BOOKEDDATE_MODEL_DATABASE_TABLE_NAME);
}

int BookedDateModel::getId(int row) {
    return this->data(this->index(row, 0), IdRole).toInt();
}

QHash<int, QByteArray> BookedDateModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id_int_pk_ai";
    roles[HotelRoomNumberRole] = "HotelRoomNumber_txt_nn";
    roles[UserLoginRole] = "UserLogin_txt_nn";
    roles[BookedDateStartRole] = "BookedDateStart_date_nn";
    roles[BookedDateEndRole] = "BookedDateEnd_date_nn";
    return roles;
}