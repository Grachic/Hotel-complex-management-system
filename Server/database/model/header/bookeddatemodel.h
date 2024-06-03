//
// Created by User on 25.05.2024.
//

#ifndef SERVER_BOOKEDDATEMODEL_H
#define SERVER_BOOKEDDATEMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include "../../../constants/constants.h"

class BookedDateModel : public QSqlQueryModel {
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1, // id
        HotelRoomIdRole,
        UserIdRole,
        BookedDateStartRole,
        BookedDateEndRole
    };

    explicit BookedDateModel(QObject* parent = nullptr);

    QVariant data(const QModelIndex & index, int role) const override;

public slots:
    void updateModel(const QString& query = "");
    int getId(int row);

protected:
    QHash<int, QByteArray> roleNames() const override;
};


#endif //SERVER_BOOKEDDATEMODEL_H
