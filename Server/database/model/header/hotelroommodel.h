//
// Created by User on 16.05.2024.
//

#ifndef SERVER_HOTELROOMMODEL_H
#define SERVER_HOTELROOMMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include "../../../constants/constants.h"

class HotelRoomModel : public QSqlQueryModel {
Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1, // id
        HotelRoomNumberRole
    };

    explicit HotelRoomModel(QObject* parent = nullptr);

    QVariant data(const QModelIndex & index, int role) const override;

public slots:
    void updateModel(const QString& query = "");
    int getId(int row);

protected:
    QHash<int, QByteArray> roleNames() const override;
};


#endif //SERVER_HOTELROOMMODEL_H
