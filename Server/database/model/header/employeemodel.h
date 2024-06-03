//
// Created by User on 16.05.2024.
//

#ifndef SERVER_EMPLOYEEMODEL_H
#define SERVER_EMPLOYEEMODEL_H

#include "usermodel.h"

class EmployeeModel : public UserModel {
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
        UserRole,                  // тип пользователя
        Salary,                    // зарплата
        Vacation,                  // даты отдыха
        Type                       // тип пользователя
    };

    explicit EmployeeModel(QObject* parent = nullptr);

private:
    QHash<int, QByteArray> roleNames() const override;
};


#endif //SERVER_EMPLOYEEMODEL_H
