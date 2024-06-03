//
// Created by User on 15.05.2024.
//

#include "../header/server.h"

Server::Server() : m_serverLogger("server"), m_nextBlockSize(0), m_clientRequestMessage("") {
    m_database.connectToDataBase();

    if (this->listen(QHostAddress::Any, SERVER_LISTENING_PORT))
        qInfo(logInfo()) << "Start listening port " << SERVER_LISTENING_PORT;
    else
        qCritical(logCritical()) << "Error in server listening port " << SERVER_LISTENING_PORT;
}

void Server::incomingConnection(qintptr socketDescriptor) {
    socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &QTcpSocket::deleteLater);

    m_sockets.push_back(socket);
    qInfo(logInfo()) << "Client connected " << socketDescriptor;
}

void Server::slotReadyRead() {
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_12);
    if (in.status() == QDataStream::Ok) {
        for(;;) {

            if (m_nextBlockSize == 0) {
                if (socket->bytesAvailable() < 2) break;
                in >> m_nextBlockSize;
            }

            if (socket->bytesAvailable() < m_nextBlockSize) {
                qInfo(logInfo(), "Data not full, break!");
                break;
            }

            in >> m_clientRequestMessage;
            m_nextBlockSize = 0;
            break;
        }
        parseClientRequest();
    }
    else {
        qCritical(logCritical()) << "DataStream error (slotReadyRead)";
    }
}

void Server::sendToClient(const QString& message) {
    m_data.clear();
    QDataStream out(&m_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << quint16(0) << message;
    out.device()->seek(0);
    out << quint16(m_data.size() - sizeof(quint16));
    out << message;
    socket->write(m_data);
}

void Server::parseClientRequest() {
    if (m_clientRequestMessage.isEmpty()) {
        qWarning(logWarning()) << "User request is empty!";
        return;
    }

    if (m_clientRequestMessage.contains("Signal(signIn)")) {
        QVariantList userData({m_clientRequestMessage.split(":::").at(1)});
        QString serverResponse = "";
        QSqlRecord record = Database::findInTable(userData, USER_MODEL_DATABASE_TABLE_NAME);
        if (record.isEmpty())
            serverResponse = "(Error)Such a user doesn`t exist!";
        else if (record.value("Password").toString() != m_clientRequestMessage.split(":::").at(2))
            serverResponse = "(Error)Incorrect password!/(Hint)" + record.value("PasswordHint").toString();
        else if (record.value("Password").toString() == m_clientRequestMessage.split(":::").at(2)) {
            /*QSqlRecord userBookedInfoRecord = Database::findInTable({record.value("id").toInt()},
                                                                    BOOKEDDATE_MODEL_DATABASE_TABLE_NAME);*/
            serverResponse.append(record.value("UserRole").toString());

            /*if (!userBookedInfoRecord.isEmpty()) {
                QString bookedRooms = "";
                for (int i = 0; userBookedInfoRecord.count(); ++i) {
                    bookedRooms.append(userBookedInfoRecord.value("HotelRoomId").toString());
                    bookedRooms.append(",");
                    bookedRooms.append(userBookedInfoRecord.value("BookedDateStart").toDate().toString());
                    bookedRooms.append(",");
                    bookedRooms.append(userBookedInfoRecord.value("BookedDateEnd").toDate().toString());
                    if (i != userBookedInfoRecord.count() - 1) bookedRooms.append("___");
                }

                serverResponse.append(":::");
                serverResponse.append(bookedRooms);
            }*/
        }
        qDebug(logInfo()) << serverResponse;
        sendToClient(serverResponse);
    }
    if (m_clientRequestMessage.contains("Signal(signUp)")) {
        QVariantList userData;

        QStringList userParameters = m_clientRequestMessage.split(":::");
        QStringList fullName = userParameters.at(1).split(" ");
        for (int i = 0; i < 3; ++i) {
            userData.append(fullName.isEmpty() ? "" : fullName.takeFirst());
        }

        for (int i = 2; i < userParameters.size(); i++) {
            userData.append(userParameters.at(i));
        }

        if (Database::insertIntoTable(userData, USER_MODEL_DATABASE_TABLE_NAME)) {
            qInfo(logInfo()) << "User: " + m_clientRequestMessage.split(":::").at(2) + " successfully registered!";
            sendToClient("You have been successfully registered!");
        } else {
            qCritical(logCritical()) << "User don`t registered!";
            sendToClient("(Error)Sorry, this login is already taken!");
        }
    }
}
