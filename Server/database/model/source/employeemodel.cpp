//
// Created by User on 16.05.2024.
//

#include "../header/employeemodel.h"

EmployeeModel::EmployeeModel(QObject* parent) : UserModel(parent) {
    updateModel("SELECT id, " EMPLOYEE_MODEL_DATABASE_PARAMETERS " FROM " EMPLOYEE_MODEL_DATABASE_TABLE_NAME);
}

QHash<int, QByteArray> EmployeeModel::roleNames() const {
    /* То есть сохраняем в хеш-таблицу названия ролей
     * по их номеру
     * */
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id_int_pk_ai";
    roles[FirstNameRole] = "FirstName_txt_nn";
    roles[SecondNameRole] = "SecondName_txt_nn";
    roles[PatronymicRole] = "Patronymic_txt";
    roles[LoginRole] = "Login_txt_nn_uq";
    roles[PasswordRole] = "Password_txt_nn";
    roles[PasswordHintRole] = "PasswordHint_txt";
    roles[PasswordRestoreRole] = "PasswordRestore_txt";
    roles[UserRole] = "UserRole_txt_nn";
    roles[Salary] = "Salary_rl_nn";
    roles[Vacation] = "Vacation_txt";
    roles[Type] = "Type_txt_nn";
    return roles;
}