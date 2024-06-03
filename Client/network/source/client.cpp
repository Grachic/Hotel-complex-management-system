//
// Created by User on 19.05.2024.
//

#include "../header/client.h"

Client::Client() : m_logger("client"), m_nextBlockSize(0), m_serverResponseMessage("") {
    m_socket = new QTcpSocket(this);

    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
    QObject::connect(m_socket, &QTcpSocket::disconnected, this, &QTcpSocket::deleteLater);
}

Client::~Client() {
    delete(m_socket);
}

void Client::connectServer() {
    m_socket->connectToHost("127.0.0.1", 2323);
}

void Client::sendToServer(const QString &message) {
    m_data.clear();
    QDataStream out(&m_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << quint16(0) << message;
    out.device()->seek(0);
    out << quint16(m_data.size() - sizeof(quint16));
    m_socket->write(m_data);
}

void Client::slotReadyRead() {
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_5_12);
    if (in.status() == QDataStream::Ok) {
        for(;;) {
            if (m_nextBlockSize == 0) {
                if (m_socket->bytesAvailable() < 2) break;
                in >> m_nextBlockSize;
            }
            if (m_socket->bytesAvailable() < m_nextBlockSize) break;

            in >> m_serverResponseMessage;
            m_nextBlockSize = 0;
            responseGot(m_serverResponseMessage);
            break;
        }
    }
    else {
        qCritical(logCritical()) << "DataStream error (slotReadyRead)";
    }
}
