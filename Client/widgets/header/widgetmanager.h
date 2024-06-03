//
// Created by User on 19.05.2024.
//

#ifndef CLIENT_WIDGETMANAGER_H
#define CLIENT_WIDGETMANAGER_H

#include <QPushButton>
#include <QMovie>
#include "../../log/header/logger.h"

#define WELCOME_PAGE 0
#define REGISTER_PAGE 1
#define USER_PAGE 2

class WidgetManager {
public:
    static Logger widget_logger;

    static void setButtonIcon(QPushButton *tempButton, const QString& iconPath);
};


#endif //CLIENT_WIDGETMANAGER_H
