//
// Created by User on 16.05.2024.
//

#ifndef SERVER_USERMODEL_H
#define SERVER_USERMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include "../../../constants/constants.h"

class UserModel : public QSqlQueryModel {
Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1, // id
        FirstNameRole,             // имя
        SecondNameRole,            // фамилия
        PatronymicRole,            // отчество
        LoginRole,                 // логин
        PasswordRole,              // пароль
        PasswordHintRole,
        PasswordRestoreRole,
        UserRole                   // тип пользователя
    };

    explicit UserModel(QObject* parent = nullptr);

    QVariant data(const QModelIndex & index, int role) const override;

public slots:
    void updateModel(const QString& query = "");
    int getId(int row);

protected:
    QHash<int, QByteArray> roleNames() const override;
};


#endif //SERVER_USERMODEL_H
