//
// Created by User on 16.05.2024.
//

#include "../header/hotelroommodel.h"

HotelRoomModel::HotelRoomModel(QObject *parent) : QSqlQueryModel(parent) {
    this->updateModel();
}

QVariant HotelRoomModel::data(const QModelIndex &index, int role) const {
    int columnId = role - Qt::UserRole - 1;
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

void HotelRoomModel::updateModel(const QString& query) {
    if (!query.isEmpty()) { this->setQuery(query); return; }
    this->setQuery("SELECT id, " HOTELROOMS_MODEL_DATABASE_PARAMETERS " FROM " HOTELROOMS_MODEL_DATABASE_TABLE_NAME);
}

int HotelRoomModel::getId(int row) {
    return this->data(this->index(row, 0), IdRole).toInt();
}

QHash<int, QByteArray> HotelRoomModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id_int_pk_ai";
    roles[HotelRoomNumberRole] = "HotelRoomNumber_txt_nn_uq";
    return roles;
}