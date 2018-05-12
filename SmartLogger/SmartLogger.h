#include <QFile>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>

/*
   Usage:
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
    SmartLogger(const char *file, int line);
    ~SmartLogger();
    QTextStream* info();
    QTextStream* warn();
    QTextStream* error();
    static void initLogger();

private:
    QTextStream *linker(QString str);

    static QSharedPointer<QTextStream> stream;
    static QSharedPointer<QFile> m_logFile;
    static QTextStream logOut;
    static QTextStream console;
    QString buffer;
    QString m_file;
    qint16 m_line;
};
