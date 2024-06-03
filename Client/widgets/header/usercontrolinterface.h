//
// Created by User on 16.05.2024.
//

#ifndef CLIENT_USERCONTROLINTERFACE_H
#define CLIENT_USERCONTROLINTERFACE_H

#include <QtWidgets>
#include "widgetmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UserControlInterface; }
QT_END_NAMESPACE

class UserControlInterface : public QWidget {
Q_OBJECT

public:
    explicit UserControlInterface(QWidget *parent = nullptr, QStringList userReservations = {});

    ~UserControlInterface() override;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::UserControlInterface *ui;

    void fillUserUI();
};

class CalendarWidget : public QCalendarWidget
{
Q_OBJECT
public:
    explicit CalendarWidget(QWidget *parent = 0);
    bool eventFilter(QObject *obj, QEvent *event) override;
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const override;

    QDate minSelectedDate() const { return minDate; }
    QDate maxSelectedDate() const { return maxDate; }

public slots:
    void setSelectedDate();

private:
    Qt::KeyboardModifiers modifier;
    QDate minDate;
    QDate maxDate;
};

#endif //CLIENT_USERCONTROLINTERFACE_H
