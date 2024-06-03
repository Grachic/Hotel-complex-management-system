//
// Created by User on 13.05.2024.
//

#ifndef HOTEL_COMPLEX_MANAGEMENT_SYSTEM_MAINWINDOW_H
#define HOTEL_COMPLEX_MANAGEMENT_SYSTEM_MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QMetaMethod>
#include <QCryptographicHash>
#include <QMessageBox>
#include "widgetmanager.h"
#include "usercontrolinterface.h"
#include "../../network/header/client.h"
#include "../../resources/extra libraries/EToastr/EToastr.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MainWindow *ui;
    Client m_networkClient;

    void initializeWelcomePage();
    void initializeRegisterPage();
    void initializeSimilarStaff();

    bool validate();
    void animationStackedWidgets();

private slots:
    void openRegisterWindow();
    void onRegisterWindowAccepted();
    void onRegisterWindowRejected();
    void onWatchPasswordButtonClicked(bool);
    void onSignInButtonClicked();
    void whenAnimationFinish();
    void getServerResponse(const QString& response);
};


#endif //HOTEL_COMPLEX_MANAGEMENT_SYSTEM_MAINWINDOW_H
