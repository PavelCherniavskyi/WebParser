#include <QTest>
#include <iostream>
#include <unistd.h>

class SmartLoggerTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void logToFileTest();
    void logToStdOutTest();
    void cleanupTestCase();
};
