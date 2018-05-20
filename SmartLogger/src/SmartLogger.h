#pragma once

#include <QFile>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>

/*
   Usage:
   Do initLogger with logOption && logFilePath;
   INFO() << "debug info";
   WARN() << "warning info";
   ERROR() << "error info";
*/

#define INFO *(SmartLogger(__FILE__, __LINE__).info)
#define WARN *(SmartLogger(__FILE__, __LINE__).warn)
#define ERROR *(SmartLogger(__FILE__, __LINE__).error)

class SmartLogger
{
public:
    enum class LOGPATH {
        LogToFile,
        LogToStdOut,
        LogBoth
    };
    SmartLogger(const char *file, int line);
    ~SmartLogger();
    QTextStream* info();
    QTextStream* warn();
    QTextStream* error();
    static void initLogger(LOGPATH logOption = SmartLogger::LOGPATH::LogBoth, QString logFilePath = QString("logFile.txt"));

private:
    QTextStream *linker(QString str);

    static QSharedPointer<QTextStream> stream;
    static QSharedPointer<QFile> m_logFile;
    static QTextStream m_logOut;
    static QTextStream m_console;
    static LOGPATH m_logPath;
    QString m_buffer;
    QString m_file;
    qint16 m_line;
};
