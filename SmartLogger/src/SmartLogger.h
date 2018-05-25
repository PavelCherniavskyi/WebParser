#pragma once

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>

class Provisioning;

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

class SmartLogger : public QObject
{
    Q_OBJECT
public:
    enum LOGWAY {
        LogToFile,
        LogToStdOut,
        LogBoth,
        NONE
    };
    Q_ENUM(LOGWAY)
    struct ProvData
    {
        LOGWAY logWay;
        QString logFilePath;
    };
    SmartLogger(const char *file, int line);
    ~SmartLogger();
    QTextStream* info();
    QTextStream* warn();
    QTextStream* error();
    static void init(Provisioning* prov);

public slots:
    static void OnProvDataReceived(SmartLogger::ProvData provData);


private:
    QTextStream *linker(QString str);

    static QSharedPointer<QTextStream> stream;
    static QSharedPointer<QFile> m_logFile;
    static QTextStream m_logOut;
    static QTextStream m_console;
    static LOGWAY m_logWay;
    static QString m_logFilePath;
    QString m_buffer;
    QString m_file;
    qint16 m_line;
};
