#include "SmartLoggerTest.h"
#include "../SmartLogger.h"
#include <QTemporaryFile>

const QString infoStr("Info test string.");
const QString warnStr("Warn test string.");
const QString errorStr("Error test string.");

void SmartLoggerTest::initTestCase()
{

}

void SmartLoggerTest::logToFileTest()
{
    SmartLogger::ProvData provData;
    QString filePath("test_log.txt");
    provData.logFilePath = filePath;
    provData.logWay = SmartLogger::LOGWAY::LogToFile;
    SmartLogger::OnProvDataReceived(provData);

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
    SmartLogger::ProvData provData;
    int filepipes[2];
    char buffer[BUFSIZ + 1];
    provData.logWay = SmartLogger::LOGWAY::LogToStdOut;
    SmartLogger::OnProvDataReceived(provData);

    INFO() << "Start test";
    QVERIFY(pipe(filepipes) == 0);

    int hdl = filepipes[1];
    int saved_stdout = dup(1);
    dup2(hdl, 1);

    INFO() << infoStr;
    WARN() << warnStr;
    ERROR() << errorStr;

    dup2(saved_stdout, 1);
    close(saved_stdout);

    int result = 3;
    read(filepipes[0], buffer, BUFSIZ);
    INFO() << "End test";
    QString line(buffer);
    if(line.contains(infoStr))
        --result;
    if(line.contains(warnStr))
        --result;
    if(line.contains(errorStr))
        --result;

    QCOMPARE(result, 0);
}

void SmartLoggerTest::cleanupTestCase()
{

}
