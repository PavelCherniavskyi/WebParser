#include "UtilsTest.h"

JobMock::JobMock()
{
    executeCalls = {0, 0, 0, 0, 1};
}

bool JobMock::execute()
{
    bool jobCompleted = executeCalls.back();
    executeCalls.pop_back();
    return jobCompleted;
}

void UtilsTest::initTestCase()
{

}

void UtilsTest::jobTest()
{
    QScopedPointer<JobMock> job(new JobMock);

}

void UtilsTest::jobExecutorTest()
{

}

void UtilsTest::cleanupTestCase()
{

}

