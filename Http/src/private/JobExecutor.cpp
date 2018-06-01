#include "JobExecutor.h"
#include "HttpJob.h"

#include "../../../SmartLogger/src/SmartLogger.h"

JobExecutor::JobExecutor()
    : QObject()
    , mWorkerThread()
    , mNewId(0)
    , mPrintDebugInfo(true)

{
    INFO() << "Creating new thread";

    mWorkerThread.setObjectName("InJobExecutorThread");
    mWorkerThread.start();
}

JobExecutor::~JobExecutor()
{
    INFO() << "Waiting for worker thread to quit";

    mWorkerThread.quit();
    mWorkerThread.wait();

    INFO() << "Worker thread has quit, active jobs:" << mActiveJobs.size();

    mActiveJobs.clear();
}


//--------------------------------------------------------------------------------------------------
void JobExecutor::execute(QSharedPointer<HttpJob> job, bool printDebugInfo)
//--------------------------------------------------------------------------------------------------
{
    mPrintDebugInfo = printDebugInfo;

    if(mPrintDebugInfo){
        INFO() << "Start job [" << mNewId << "]: '" << job->name() << "'";
    }

    job->setId(mNewId++);

    connect(this,      &JobExecutor::executeJob, job.data(), &HttpJob::executeJob,                 Qt::QueuedConnection);
    connect(job.data(), &HttpJob::executionCompleted, this,      &JobExecutor::executionCompleted, Qt::QueuedConnection);

    mActiveJobs.append(job);

    job->moveToThread(&mWorkerThread);

    if(mPrintDebugInfo){
        INFO() << "Thread state:" << mWorkerThread.isRunning();
    }

    emit executeJob();

    // disconnect because we need to send signal to Job once
    disconnect(this, &JobExecutor::executeJob, job.data(), &HttpJob::executeJob);
}

//--------------------------------------------------------------------------------------------------
void JobExecutor::executionCompleted(int32_t id)
//--------------------------------------------------------------------------------------------------
{
    foreach(QSharedPointer<HttpJob> job, mActiveJobs) {
        if (job->id() == id) {
            if(mPrintDebugInfo){
                INFO() << "Job [" << id << "]: '" << job->name() << "' completed";
            }
            if (!mActiveJobs.removeOne(job)) {
                WARN() << "Can not remove job [" << id << "]";
            }
            return;
        }
    }
    WARN() << "Wrong JobId = " << id;
}
