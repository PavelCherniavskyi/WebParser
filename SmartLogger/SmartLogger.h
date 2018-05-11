#include <QFile>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>

//Usage:
//qInfo() << "Info Button";
//qDebug() << "Debug Button";
//qWarning() << "Warning Button";
//qCritical() << "Critical Button";

//To use verbose default bebug:
//qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz} %{type}] %{file}:%{line} - %{message}");

#define INFO SmartLogger(__FILE__, __LINE__, QT_MESSAGELOG_FUNC).info
//#define WARN SmartLogger(__FILE__, __LINE__, QT_MESSAGELOG_FUNC).warning
//#define ERROR SmartLogger(__FILE__, __LINE__, QT_MESSAGELOG_FUNC).critical

class SmartLogger
{
public:
    SmartLogger(const char *file, int line);
    void initLogger();
};
