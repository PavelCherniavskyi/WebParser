#include "SmartLoggerTest.h"

const QString infoStr("Info test string.");
const QString warnStr("Warn test string.");
const QString errorStr("Error test string.");

void SmartLoggerTest::initTestCase()
{

}

void SmartLoggerTest::logToFileTest()
{
    QString filePath("test_log.txt");
    SmartLogger::init(SmartLogger::LOGWAY::LogToFile, filePath);

    INFO() << infoStr;
    WARN() << warnStr;
    ERROR() << errorStr;

    QFile file(filePath);

    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Truncate));

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

    QCOMPARE(result, 0);
}

void SmartLoggerTest::logToStdOutTest()
{
    SmartLogger::init(SmartLogger::LOGWAY::LogToStdOut);
    QFile fileOut("test_file.txt");

    QVERIFY(fileOut.open(QIODevice::ReadWrite | QIODevice::Truncate));

    int hdl = fileOut.handle();
    int saved_stdout = dup(1);
    dup2(hdl, 1);

    INFO() << infoStr;
    WARN() << warnStr;
    ERROR() << errorStr;

    fileOut.seek(0);

    int result = 3;
    QTextStream stream(&fileOut);
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

    dup2(saved_stdout, 1);
    close(saved_stdout);
    fileOut.close();

    QCOMPARE(result, 0);
}

void SmartLoggerTest::cleanupTestCase()
{

}
