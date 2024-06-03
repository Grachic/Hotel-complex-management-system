//
// Created by User on 13.05.2024.
//
#include <QApplication>
#include "widgets/header/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
