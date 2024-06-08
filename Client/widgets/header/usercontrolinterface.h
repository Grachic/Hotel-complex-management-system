//
// Created by User on 16.05.2024.
//

#ifndef CLIENT_USERCONTROLINTERFACE_H
#define CLIENT_USERCONTROLINTERFACE_H

#include <QtWidgets>
#include "widgetmanager.h"
#include "../../network/header/client.h"
#include "../../resources/extra libraries/EToastr/EToastr.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UserControlInterface; }
QT_END_NAMESPACE

struct UserProfile {
    QString Login;
    QString Password;
    QString PasswordHint;
    QString PasswordRestore;
    QMap<QString, QString> Reservations;
};

class UserControlInterface : public QWidget {
Q_OBJECT

public:
    explicit UserControlInterface(QWidget *parent = nullptr, Client *networkClient = nullptr, UserProfile profile = {});

    ~UserControlInterface() override;

    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
    void onChoseDateButtonClicked();
    void onCurrentFloorChanged(const QString&);
    void onRoomButtonClicked();

private slots:
    void getServerResponse(const QString &response);

private:
    Ui::UserControlInterface *ui;
    Client *m_networkClient;

    QStringList m_reservedRooms;
    UserProfile m_userProfile;

    void fillUserUI();
};

//class ReservationWidget : public QTableVi

class CalendarWidget : public QCalendarWidget
{
Q_OBJECT
public:
    explicit CalendarWidget(QWidget *parent = nullptr);
    bool eventFilter(QObject *obj, QEvent *event) override;
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const override;

    QDate minSelectedDate() const { return minDate; }
    QDate maxSelectedDate() const { return maxDate; }

public slots:
    void setSelectedDate();

private:
    Qt::KeyboardModifiers modifier;
    QDate minDate {};
    QDate maxDate {};
};

#endif //CLIENT_USERCONTROLINTERFACE_H
