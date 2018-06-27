#pragma once

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>
#include <QMetaEnum>
#include <QMutex>
#include <QProcess>

class Provisioning;

/*
   Usage:
   Do initLogger with logOption && logFilePath;
   INFO() << "debug info";
   WARN() << "warning info";
   ERROR() << "error info";
*/

#define INFO *(SmartLogger(__FILE__, __FUNCTION__, __LINE__).info)
#define WARN *(SmartLogger(__FILE__, __FUNCTION__, __LINE__).warn)
#define ERROR *(SmartLogger(__FILE__, __FUNCTION__, __LINE__).error)

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
        bool isDLTEnabled;
    };
    SmartLogger(const char *file, const char *function, int line);
    ~SmartLogger();
    QTextStream* info();
    QTextStream* warn();
    QTextStream* error();
    static void init(Provisioning* prov);

public slots:
    static void OnProvDataReceived(SmartLogger::ProvData provData);


private:
    QTextStream *linker(QString str);
    static void startDLTDeamon();

    static QScopedPointer<QTextStream> stream;
    static QScopedPointer<QFile> m_logFile;
    static QTextStream m_logOut;
    static QTextStream m_console;
    static LOGWAY m_logWay;
    static QString m_logFilePath;
    static QMutex mutex;
    static bool m_isDLTEnabled;
    static QProcess *m_dlt_daemon_proc;
    QString m_buffer;
    QString m_file;
    qint16 m_line;
    QString m_function;
};

template<typename QEnum>
QString EnumToQStr(const QEnum value)
{
    return QString(QMetaEnum::fromType<QEnum>().valueToKey(value));
}
