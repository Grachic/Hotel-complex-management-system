//
// Created by User on 16.05.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_UserControlInterface.h" resolved

#include "../header/usercontrolinterface.h"
#include "../ui/ui_UserControlInterface.h"

UserControlInterface::UserControlInterface(QWidget *parent, Client *networkClient, UserProfile profile) :
        QWidget(parent), m_networkClient(networkClient), m_userProfile(profile), ui(new Ui::UserControlInterface) {
    ui->setupUi(this);

    connect(&*m_networkClient, &Client::responseGot,
            this, &UserControlInterface::getServerResponse);

    fillUserUI();

    auto *qssFile = new QFile(":/qss/resources/stylesheet/usercontrolinterface.qss");
    qssFile->open(QFile::ReadOnly);
    this->setStyleSheet(QTextStream(qssFile).readAll());
}

UserControlInterface::~UserControlInterface() {
    delete ui;
}

void UserControlInterface::fillUserUI() {
    auto *calendarWidget = new CalendarWidget();
    calendarWidget->setObjectName("reservationCalendarWidget");
    calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendarWidget->setMinimumDate(QDate::currentDate());
    calendarWidget->setMaximumDate(QDate(QDate::currentDate().year(), 12, 31));
    ui->reservationGrid->addWidget(calendarWidget, 0, 0);

    ui->hotelMapFrame->setEnabled(false);

    connect(ui->choseDateButton, &QPushButton::clicked,
            this, &UserControlInterface::onChoseDateButtonClicked);
    connect(ui->floorComboBox, &QComboBox::currentTextChanged,
            this, &UserControlInterface::onCurrentFloorChanged);
    for (int i = 1; i <= 20; ++i)
        connect(this->findChild<QPushButton*>("room" + QString::number(i) + "Button"), &QPushButton::clicked,
                this, &UserControlInterface::onRoomButtonClicked);

    for (auto button: this->findChildren<QPushButton *>()) {
        QString iconPath = "";
        if (button->objectName().contains("profile"))
            iconPath = "../assets/gif/profile_icon.gif";
        if (button->objectName().contains("quit"))
            iconPath = "../assets/gif/quit_icon.gif";
        if (button->objectName().contains("room"))
            iconPath = "../assets/gif/free_room_icon.gif";
        if (button->objectName().contains("Ladder"))
            iconPath = "../assets/gif/ladder_icon.gif";
        if (button->objectName().contains("PassengerElevator"))
            iconPath = "../assets/gif/passenger_elevator_icon.gif";
        if (button->objectName().contains("FreightElevator"))
            iconPath = "../assets/gif/freight_elevator_icon.gif";

        if (!iconPath.isEmpty()) {
            WidgetManager::setButtonIcon(button, iconPath);
            button->installEventFilter(this);
        }
    }
}

bool UserControlInterface::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::HoverEnter) {
        QString iconPath = "";
        if (obj->objectName().contains("profile"))
            iconPath = "../assets/gif/profile_icon.gif";
        if (obj->objectName().contains("quit"))
            iconPath = "../assets/gif/quit_icon.gif";
        if (obj->objectName().contains("room"))
            iconPath = "../assets/gif/free_room_icon.gif";
        if (obj->objectName().contains("Ladder"))
            iconPath = "../assets/gif/ladder_icon.gif";
        if (obj->objectName().contains("PassengerElevator"))
            iconPath = "../assets/gif/passenger_elevator_icon.gif";
        if (obj->objectName().contains("FreightElevator"))
            iconPath = "../assets/gif/freight_elevator_icon.gif";

        if (!iconPath.isEmpty()) {
            WidgetManager::setButtonIcon((QPushButton *) obj, iconPath);
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void UserControlInterface::onChoseDateButtonClicked() {
    auto *calendar = this->findChild<CalendarWidget*>("reservationCalendarWidget");
    QString dateRange = calendar->minSelectedDate().toString(Qt::DateFormat::DefaultLocaleShortDate) + "-" +
            calendar->maxSelectedDate().toString(Qt::DateFormat::DefaultLocaleShortDate);

    ui->chosedDateLabel->setText(dateRange);
    m_networkClient->sendToServer("Signal(getReserveMap):::" + dateRange);
}

void UserControlInterface::getServerResponse(const QString &response) {
    if (response.contains("(Error)")) {
        ui->chosedDateLabel->setText(response.split("/").first().remove("(Error)"));
        return;
    }
    if (response.contains("(Success)")) {
        auto* toastr = new EToastr(reinterpret_cast<EWidget *>(this));
        toastr->setStyle(EToastr::TOASTR_STYLE::SUCCESS);
        toastr->setText(response);
        toastr->show(EToastr::TOASTR_DIRECTION::TOP_TO_BOTTOM);

        m_networkClient->sendToServer("Signal(getReserveMap):::" + ui->chosedDateLabel->text());
        return;
    }

    ui->hotelMapFrame->setEnabled(true);

    if (m_reservedRooms.isEmpty()) {
        int floorCount = response.split(":::").first().toInt();
        QStringList floors; for(int i = 1; i < floorCount + 1; ++i) floors << QString::number(i) + " floor";
        ui->floorComboBox->addItems(floors);
    }

    if (response.split(":::").count() == 1) return;

    m_reservedRooms = response.split(":::").last().split("/");
}

void UserControlInterface::onCurrentFloorChanged(const QString &currentText) {
    for (auto &roomData : m_reservedRooms) {
        QString roomIndex = roomData.split(",").first().split("_").last();
        if (roomIndex == currentText)
            ui->hotelMapFrame->findChild<QPushButton*>("room" + roomIndex + "Button")->setEnabled(false);
    }
}

void UserControlInterface::onRoomButtonClicked() {
    auto *senderButton = (QPushButton*)sender();
    QString roomNumber = QString(ui->floorComboBox->currentText()).remove(QRegExp("\\D")) +
            "_" +
            QString(senderButton->objectName()).remove(QRegExp("\\D"));

    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                             "Room " + roomNumber,
                                                             "Do you really want to book room " + roomNumber + "\nbetween " + ui->chosedDateLabel->text() + "?",
                                                             QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QString clientRequest = "";
        clientRequest.append("Signal(setReserveMap)");
        clientRequest.append(":::");
        clientRequest.append(roomNumber);
        clientRequest.append(":::");
        clientRequest.append(m_userProfile.Login);
        clientRequest.append(":::");
        clientRequest.append(ui->chosedDateLabel->text());
        m_networkClient->sendToServer(clientRequest);
    }
}

CalendarWidget::CalendarWidget(QWidget *parent) :
        QCalendarWidget(parent)
{
    auto *view = this->findChild<QTableView *>();
    view->viewport()->installEventFilter(this);
    view->installEventFilter(this);
    connect(this,SIGNAL(selectionChanged()),this,SLOT(setSelectedDate()));
    minDate = maxDate = QDate::currentDate();
}

void CalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    if ((date >= minDate && date <= maxDate))
    {
        if (date == minDate || date == maxDate) painter->fillRect (rect, "#9a7668");
        else painter->fillRect (rect, "#dcccc5");
        painter->drawText(rect, Qt::AlignCenter | Qt::AlignHCenter,
                          QString::number(date.day()));
        return;
    }

    QCalendarWidget::paintCell(painter,rect,date);
}

void CalendarWidget::setSelectedDate()
{
    QDate date = selectedDate();
    if( modifier & Qt::ShiftModifier )
    {
        minDate = qMin(minDate,date);
        maxDate = qMax(maxDate,date);
    }
    else
        minDate = maxDate = date;
    updateCells();
}

bool CalendarWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonRelease)
        modifier = 	dynamic_cast<QMouseEvent*>(event)->modifiers();
    if(event->type() == QEvent::KeyPress)
        modifier = 	dynamic_cast<QKeyEvent*>(event)->modifiers();
    return QCalendarWidget::eventFilter(obj, event);
}