#include "UtilsTest.h"
#include "SmartLogger.h"

JobMock::JobMock()
{
    executeCalls = {0, 1, 1, 1, 1};
}

bool JobMock::execute()
{

    bool jobCompleted = executeCalls.back();
    executeCalls.pop_back();
    INFO() << "execute " << jobCompleted;
    return jobCompleted;
}

void JobMock::executionCompleted(int32_t id)
{
    INFO() << "executionCompleted " << id;
    QCOMPARE(id, 98);
}

void UtilsTest::initTestCase()
{

}

void UtilsTest::jobTest()
{
    QScopedPointer<JobMock> job(new JobMock);
    job->setId(99);
    connect(job.data(), &Job::executionCompleted, job.data(), &JobMock::executionCompleted, Qt::QueuedConnection);
    job->executeJob();
    QCoreApplication::processEvents();
    QTest::qSleep(500);
    INFO();
}

void UtilsTest::jobExecutorTest()
{

}

void UtilsTest::cleanupTestCase()
{

}

