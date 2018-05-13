#include "../SmartLogger.h"

/*
  TODO:
  1. Implement logToStdOutTest.
    Problem: can't get current running QProcess to read from its stdout or somehow read from current stdout.
  2. Implement logToBothTest.
*/

class SmartLoggerTest
{
public:
    int run();
private:
    void initTestCase();
    int logToFileTest();
    //int logToStdOutTest();
    //int logToBothTest();
    void cleanupTestCase();
};
