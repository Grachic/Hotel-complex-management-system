//
// Created by User on 13.05.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "../header/mainwindow.h"
#include "../ui/ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_networkClient.connectServer();

    auto *qssFile = new QFile(":/qss/resources/stylesheet/mainwindow.qss");
    qssFile->open(QFile::ReadOnly);
    this->setStyleSheet(QTextStream(qssFile).readAll());

    initializeSimilarStaff();
    initializeWelcomePage();
    initializeRegisterPage();
}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj->objectName().contains("Watch") &&
        obj->objectName().toLower().contains("password")) {
        if (event->type() == QEvent::HoverEnter) {
            WidgetManager::setButtonIcon((QPushButton *) obj, "../assets/gif/watch_password_icon.gif");
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void MainWindow::initializeSimilarStaff() {
    for (auto button: this->findChildren<QPushButton *>()) {
        if (button->objectName().contains("Watch") && button->objectName().toLower().contains("password")) {
            button->setCheckable(true);
            WidgetManager::setButtonIcon(button, "../assets/gif/watch_password_icon.gif");
            button->installEventFilter(this);
            connect(button, SIGNAL(toggled(bool)),
                    this, SLOT(onWatchPasswordButtonClicked(bool)));
        }
    }
}

void MainWindow::initializeWelcomePage() {
    ui->loginLineEdit_welcome->setProperty("required", true);
    ui->passwordLineEdit_welcome->setProperty("required", true);

    connect(ui->signInButton_welcome, SIGNAL(clicked()),
            this, SLOT(onSignInButtonClicked()));
    connect(ui->signUpButton_welcome, SIGNAL(clicked()),
            this, SLOT(openRegisterWindow()));

    connect(&m_networkClient, &Client::responseGot,
            this, &MainWindow::getServerResponse);
}

void MainWindow::initializeRegisterPage() {
    ui->fullNameLineEdit_register->setProperty("required", true);
    ui->loginLineEdit_register->setProperty("required", true);
    ui->passwordLineEdit_register->setProperty("required", true);
    ui->repeatPasswordLineEdit_register->setProperty("required", true);

    ui->buttonBox_register->button(QDialogButtonBox::Ok)->setText("Register");

    connect(ui->buttonBox_register, SIGNAL(rejected()),
            this, SLOT(onRegisterWindowRejected()));

    connect(ui->buttonBox_register, SIGNAL(accepted()),
            this, SLOT(onRegisterWindowAccepted()));
}

void MainWindow::openRegisterWindow() {
    animationStackedWidgets();
    ui->centralStackedWidget->setCurrentIndex(REGISTER_PAGE);
}

void MainWindow::animationStackedWidgets() {
    auto *effect = new QGraphicsOpacityEffect(this);
    ui->centralStackedWidget->setGraphicsEffect(effect);
    auto *anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(870);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::InCubic);
    anim->start(QPropertyAnimation::DeleteWhenStopped);

    connect(anim, SIGNAL(finished()), this, SLOT(whenAnimationFinish()));
}

void MainWindow::whenAnimationFinish() {
    ui->centralStackedWidget->setGraphicsEffect(nullptr);
}

void MainWindow::onRegisterWindowAccepted() {
    if (validate()) {
        m_networkClient.sendToServer("Signal(signUp):::" +
                                     ui->fullNameLineEdit_register->text() + ":::" +
                                     ui->loginLineEdit_register->text() + ":::" +
                                     QCryptographicHash::hash(ui->passwordLineEdit_register->text().toLocal8Bit(),
                                                              QCryptographicHash::Sha256) + ":::" +
                                     ui->auxillaryLineEdit_register->text() + ":::" +
                                     ui->forgotPasswordLineEdit_register->text() + ":::" +
                                     "User");
    }
}

void MainWindow::onRegisterWindowRejected() {
    animationStackedWidgets();
    ui->centralStackedWidget->setCurrentIndex(WELCOME_PAGE);
}

void MainWindow::onWatchPasswordButtonClicked(bool arg) {
    QString lineEditName =
            sender()->objectName().split("Watch").first() + "LineEdit_" + sender()->objectName().split("_").last();
    this->findChild<QLineEdit *>(lineEditName)->setEchoMode(arg ? QLineEdit::Normal : QLineEdit::Password);
}

void MainWindow::onSignInButtonClicked() {
    if (validate()) {
        m_networkClient.sendToServer("Signal(signIn):::" +
                                     ui->loginLineEdit_welcome->text() + ":::" +
                                     QCryptographicHash::hash(ui->passwordLineEdit_welcome->text().toLocal8Bit(),
                                                              QCryptographicHash::Sha256));
    }
}

void MainWindow::getServerResponse(const QString &response) {
    qDebug(logDebug()) << response;
    if (response.contains("(Error)")) {
        for (auto &errorLabel: ui->centralStackedWidget->currentWidget()->findChildren<QLabel *>()) {
            if (errorLabel->objectName().contains("errorLabel")) {
                errorLabel->setText(response.split("/").first().remove("(Error)"));
                return;
            }
        }
    } else {
        if (ui->centralStackedWidget->currentIndex() == REGISTER_PAGE) {
            animationStackedWidgets();
            ui->centralStackedWidget->setCurrentIndex(WELCOME_PAGE);

            auto* toastr = new EToastr(reinterpret_cast<EWidget *>(this));
            toastr->setStyle(EToastr::TOASTR_STYLE::SUCCESS);
            toastr->setText("You have been successfully registered!");
            toastr->show(EToastr::TOASTR_DIRECTION::TOP_TO_BOTTOM);

            for (auto lineEdit : ui->centralStackedWidget->widget(REGISTER_PAGE)->findChildren<QLineEdit*>()) {
                lineEdit->setText("");
                lineEdit->setStyleSheet("border-color: none;");
            }
            return;
        }

        if (ui->centralStackedWidget->currentIndex() == WELCOME_PAGE) {
            int pageIndex = WELCOME_PAGE;
            if (response.contains("User")) {
                auto *userUi = new UserControlInterface(ui->centralStackedWidget);
                ui->centralStackedWidget->insertWidget(USER_PAGE, userUi);
                pageIndex = USER_PAGE;
            }
            animationStackedWidgets();
            ui->centralStackedWidget->setCurrentIndex(pageIndex);
            return;
        }
    }
}

bool MainWindow::validate() {
    bool isThereRequiredEmptyFields = false;
    bool isThereNotRequiredEmptyFields = false;
    for (auto lineEdit: ui->centralStackedWidget->currentWidget()->findChildren<QLineEdit *>()) {
        if (lineEdit->property("required").toBool() && lineEdit->text().isEmpty()) {
            isThereRequiredEmptyFields = true;
            lineEdit->setStyleSheet("border-style: outset;"
                                    "border-width: 3px;"
                                    "border-color: #b00303;");
        } else if (lineEdit->text().isEmpty()) {
            isThereNotRequiredEmptyFields = true;
            lineEdit->setStyleSheet("border-style: outset;"
                                    "border-width: 3px;"
                                    "border-color: #e0c200;");
        } else {
            lineEdit->setStyleSheet("border-color: none;");
        }
    }

    if (ui->centralStackedWidget->currentIndex() == WELCOME_PAGE) {
        if (isThereRequiredEmptyFields) {
            ui->errorLabel_welcome->setText("Empty fields!");
            return false;
        } else ui->errorLabel_welcome->setText("");
    } else if (ui->centralStackedWidget->currentIndex() == REGISTER_PAGE) {
        if (isThereRequiredEmptyFields) {
            ui->errorLabel_register->setText("Empty fields!");
            return false;
        } else ui->errorLabel_register->setText("");

        if (ui->passwordLineEdit_register->text() != ui->repeatPasswordLineEdit_register->text()) {
            ui->errorLabel_register->setText("Passwords don`t match!");
            return false;
        } else ui->errorLabel_register->setText("");

        if (isThereNotRequiredEmptyFields) {
            bool isUserWarned = !ui->warningLabel_register->text().isEmpty();
            ui->warningLabel_register->setText("If these field are not filled in,\n"
                                               "it will be more difficult for you to recover your password");
            return isUserWarned;
        } else ui->warningLabel_register->setText("");
    }

    return true;
}