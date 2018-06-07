#include "UtilsTest.h"
#include "SmartLogger.h"
#include "ExpressServer.h"
#include <curl/curl.h>
#include <QSignalSpy>
#include <QStringList>
#include <QDir>

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

void UtilsTest::expressServerTest()
{
    QString arg = QDir::currentPath();
    arg += "/../../WebParser/Utils/NodeJsExpress/app.js";
    QStringList arguments = {arg};
    ExpressServer expressServer(arguments, this);
    QVERIFY(expressServer.startServer());

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/");
        curl_easy_setopt(curl, CURLOPT_PORT, 3000L);

        res = curl_easy_perform(curl);

        QCOMPARE(res, CURLE_OK);

        curl_easy_cleanup(curl);
    }
}

void UtilsTest::cleanupTestCase()
{
}

