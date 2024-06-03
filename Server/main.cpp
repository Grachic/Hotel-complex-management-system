//
// Created by User on 15.05.2024.
//

#include <QCoreApplication>
#include "database/header/database.h"
#include "network/header/server.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    Server server;

    return QCoreApplication::exec();
}