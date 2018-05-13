#include "SmartLoggerTest.h"
#include <QDir>
#include <QProcess>

const QString infoStr("Info test string.");
const QString warnStr("Warn test string.");
const QString errorStr("Error test string.");

int main()
{
    SmartLoggerTest test;
    return test.run();
}

int SmartLoggerTest::run()
{
    int result;

    initTestCase();
    result = logToFileTest();
    cleanupTestCase();

    return result;
}

void SmartLoggerTest::initTestCase()
{

}

int SmartLoggerTest::logToFileTest()
{
    QString filePath(QDir::currentPath() + "/test_log.txt");
    SmartLogger::initLogger(SmartLogger::LOGPATH::LogToFile, filePath);

    INFO() << infoStr;
    WARN() << warnStr;
    ERROR() << errorStr;

    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open file.";
        return 1;
    }

    int result = 3;
    QTextStream stream(&file);
    QString line = stream.readLine();
    while (!line.isNull()) {
       if(line.contains(infoStr))
           --result;
       else if(line.contains(warnStr))
           --result;
       else if(line.contains(errorStr))
           --result;
       line = stream.readLine();
    }

    file.close();
    return result;
}

void SmartLoggerTest::cleanupTestCase()
{

}
