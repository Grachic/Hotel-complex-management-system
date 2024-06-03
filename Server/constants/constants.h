//
// Created by User on 16.05.2024.
//

#ifndef SERVER_CONSTANTS_H
#define SERVER_CONSTANTS_H

/// SERVER
#define SERVER_LISTENING_PORT 2323

/// DATABASE
#define DATABASE_HOSTNAME   "HotelDataBase"
#define DATABASE_NAME       "hotel.db"
// Accounting
#define ALL_DATABASE_TABLES_NAMES "Users, Employees, HotelRooms, BookedData"

/// USER
#define USER_MODEL_DATABASE_PARAMETERS "FirstName, SecondName, Patronymic, Login, Password, PasswordHint, PasswordRestore, UserRole"
#define USER_MODEL_DATABASE_PARAMETERS_KEYS ":FName, :SName, :Patronymic, :Login, :Password, :PasswordH, :PasswordR, :Role"
#define USER_MODEL_DATABASE_TABLE_NAME "Users"

/// EMPLOYEE
#define EMPLOYEE_MODEL_DATABASE_PARAMETERS "FirstName, SecondName, Patronymic, Login, Password, PasswordHint, PasswordRestore, Role, Salary, Vacation, Type"
#define EMPLOYEE_MODEL_DATABASE_PARAMETERS_KEYS ":FName, :SName, :Patronymic, :Login, :Password, :PasswordH, :PasswordR, :Role, :Salary, :Vacation, :Type"
#define EMPLOYEE_MODEL_DATABASE_TABLE_NAME "Employees"

/// HOTEL ROOMS
#define HOTELROOMS_MODEL_DATABASE_PARAMETERS "HotelRoomNumber"
#define HOTELROOMS_MODEL_DATABASE_PARAMETERS_KEYS ":HotelRoomNumber"
#define HOTELROOMS_MODEL_DATABASE_TABLE_NAME "HotelRooms"

#define HOTELROOMS_DATABASE_COUNT_ON_ONE_FLOOR 20
#define HOTELROOMS_DATABASE_COUNT_FLOORS 5

/// BOOKED DATE
#define BOOKEDDATE_MODEL_DATABASE_PARAMETERS "HotelRoomId, UserId, BookedDateStart, BookedDateEnd"
#define BOOKEDDATE_MODEL_DATABASE_PARAMETERS_KEYS ":HotelRoomId, :UserId, :BookedDateStart, :BookedDateEnd"
#define BOOKEDDATE_MODEL_DATABASE_TABLE_NAME "BookedData"

#endif //SERVER_CONSTANTS_H
