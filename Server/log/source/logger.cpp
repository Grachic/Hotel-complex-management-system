//
// Created by User on 15.05.2024.
//

#include "../header/logger.h"

Q_LOGGING_CATEGORY(logDebug, "Debug")
Q_LOGGING_CATEGORY(logInfo, "Info")
Q_LOGGING_CATEGORY(logWarning, "Warning")
Q_LOGGING_CATEGORY(logCritical, "Critical")

QScopedPointer<QFile> Logger::m_logFile(new QFile());

Logger::Logger(const QString& fileName) {
    // Устанавливаем файл логирования,
    // внимательно сверьтесь с тем, какой используете путь для файла
    QDir logDirectory(QDir::currentPath());
    logDirectory.mkpath("log/output");
    m_logFile.reset(new QFile(logDirectory.path() + "/log/output/" + fileName + "_logs.txt"));
    // Открываем файл логирования
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Устанавливаем обработчик
    qInstallMessageHandler(messageHandler);
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    // Открываем поток записи в файл
    QTextStream out(m_logFile.data());
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
        case QtInfoMsg:     out << "INF "; break;
        case QtDebugMsg:    out << "DBG "; break;
        case QtWarningMsg:  out << "WRN "; break;
        case QtCriticalMsg: out << "CRT "; break;
        case QtFatalMsg:    out << "FTL "; break;
    }
    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": "
        << msg << endl;
    out.flush();    // Очищаем буферизированные данные
}
