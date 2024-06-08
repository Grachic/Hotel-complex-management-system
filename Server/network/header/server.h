//
// Created by User on 15.05.2024.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QRegularExpression>
#include <QSqlField>
#include "../../log/header/logger.h"
#include "../../constants/constants.h"
#include "../../database/header/database.h"

class Server : public QTcpServer {
    Q_OBJECT

public:
    Server();
    ~Server() override;
    QTcpSocket *socket{};

    void sendToClient(const QString& message);

public slots:
    void slotReadyRead();
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QVector<QTcpSocket*> m_sockets;
    QByteArray m_data;
    quint16  m_nextBlockSize;
    QString m_clientRequestMessage;

    Logger m_serverLogger;

    Database m_database;

    void parseClientRequest();
};


#endif //SERVER_SERVER_H
