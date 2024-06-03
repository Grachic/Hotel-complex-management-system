//
// Created by User on 15.05.2024.
//

#ifndef CLIENT_LOGGER_H
#define CLIENT_LOGGER_H

#include <QFile>
#include <QLoggingCategory>
#include <QDateTime>
#include <QDir>

Q_DECLARE_LOGGING_CATEGORY(logDebug)
Q_DECLARE_LOGGING_CATEGORY(logInfo)
Q_DECLARE_LOGGING_CATEGORY(logWarning)
Q_DECLARE_LOGGING_CATEGORY(logCritical)

class Logger {
public:
    explicit Logger(const QString& fileName);
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static QScopedPointer<QFile> m_logFile;
};


#endif //CLIENT_LOGGER_H
