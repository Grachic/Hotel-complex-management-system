//
// Created by User on 19.05.2024.
//

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <QDataStream>
#include <QTcpSocket>
#include "../../log/header/logger.h"

class Client : public QObject {
Q_OBJECT

public:
    Client();
    ~Client() override;

    void connectServer();
    void sendToServer(const QString &message);

signals:
    void responseGot(const QString&);

public slots:
    void slotReadyRead();

private:
    QTcpSocket *m_socket;
    QByteArray m_data;
    quint16 m_nextBlockSize;
    QString m_serverResponseMessage;

    Logger m_logger;
};


#endif //CLIENT_CLIENT_H
