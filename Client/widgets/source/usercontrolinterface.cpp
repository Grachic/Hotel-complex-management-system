//
// Created by User on 16.05.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_UserControlInterface.h" resolved

#include "../header/usercontrolinterface.h"
#include "../ui/ui_UserControlInterface.h"

UserControlInterface::UserControlInterface(QWidget *parent, QStringList userReservations) :
        QWidget(parent), ui(new Ui::UserControlInterface) {
    ui->setupUi(this);

    auto *qssFile = new QFile(":/qss/resources/stylesheet/usercontrolinterface.qss");
    qssFile->open(QFile::ReadOnly);
    this->setStyleSheet(QTextStream(qssFile).readAll());

    fillUserUI();
}

UserControlInterface::~UserControlInterface() {
    delete ui;
}

void UserControlInterface::fillUserUI() {
    auto *calendarWidget = new CalendarWidget();
    calendarWidget->setMinimumDate(QDate::currentDate());
    calendarWidget->setMaximumDate(QDate(QDate::currentDate().year(), 12, 31));
    calendarWidget->setParent(ui->reservationFrame);

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
    if((date >= minDate && date <= maxDate) && date != selectedDate() )
    {
        painter->fillRect (rect, Qt::yellow);
        painter->drawText (rect, Qt::AlignCenter | Qt::AlignHCenter,
                           QString::number(date.day ()));
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