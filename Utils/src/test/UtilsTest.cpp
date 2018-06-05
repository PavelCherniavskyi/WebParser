#include "UtilsTest.h"
#include "SmartLogger.h"
#include <QSignalSpy>

quint32 JobId = 99;

JobMock::JobMock()
{
    executeCalls = {0, 1, 1, 1, 1};
    callNumber = executeCalls.size();
}

bool JobMock::execute()
{
    callNumber--;
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
    QSignalSpy spy(job.data(), &Job::executionCompleted);
    job->setId(JobId);
    QCOMPARE(job->id(), JobId);

    job->executeJob();
    QCOMPARE(job->calls(), 0);
    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == JobId);
}

void UtilsTest::jobExecutorTest()
{
    JobExecutor jobExecutor;
    QSharedPointer<JobMock> job(new JobMock, &QObject::deleteLater);
    QSignalSpy spy(&jobExecutor, &JobExecutor::jobIsDone);
    jobExecutor.execute(job, false);
    QCOMPARE(jobExecutor.currentActiveJobs(), 1);
    QVERIFY(spy.wait(500));

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == 0); //index of job. As we create a new JobExecutor first job should be 0.
    QCOMPARE(jobExecutor.currentActiveJobs(), 0);
}

void UtilsTest::cleanupTestCase()
{
}

