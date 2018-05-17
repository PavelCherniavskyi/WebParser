#include "../SmartLogger.h"
#include <QTest>
#include <iostream>
#include <unistd.h>

/*
  TODO:
  1. Implement logToStdOutTest.
    Problem: can't get current running QProcess to read from its stdout or somehow read from current stdout.
  2. Implement logToBothTest.
*/

class SmartLoggerTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void logToFileTest();
    void logToStdOutTest();
    void logToBothTest();
    void cleanupTestCase();
};
