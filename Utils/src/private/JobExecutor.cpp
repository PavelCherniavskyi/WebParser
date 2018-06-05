#include "../JobExecutor.h"
#include "../Job.h"

#include "../../../SmartLogger/src/SmartLogger.h"

JobExecutor::JobExecutor()
    : QObject()
    , mWorkerThread()
    , mNewId(0)
    , mPrintDebugInfo(true)

{
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
void JobExecutor::execute(QSharedPointer<Job> job, bool printDebugInfo)
//--------------------------------------------------------------------------------------------------
{
    mPrintDebugInfo = printDebugInfo;

    if(mPrintDebugInfo){
        INFO() << "Start job [" << mNewId << "]";
    }

    job->setId(mNewId++);

    connect(this, &JobExecutor::executeJob, job.data(), &Job::executeJob, Qt::QueuedConnection);
    connect(job.data(), &Job::executionCompleted, this, &JobExecutor::executionCompleted, Qt::QueuedConnection);

    mActiveJobs.append(job);

    job->moveToThread(&mWorkerThread);

    if(mPrintDebugInfo){
        INFO() << "Thread state:" << mWorkerThread.isRunning();
    }

    emit executeJob();

    // disconnect because we need to send signal to Job once
    disconnect(this, &JobExecutor::executeJob, job.data(), &Job::executeJob);
}

//--------------------------------------------------------------------------------------------------
void JobExecutor::executionCompleted(quint32 id)
//--------------------------------------------------------------------------------------------------
{
    foreach(QSharedPointer<Job> job, mActiveJobs) {
        if (job->id() == id) {
            if(mPrintDebugInfo){
                INFO() << "Job [" << id << "]: completed";
            }
            if (!mActiveJobs.removeOne(job)) {
                WARN() << "Can not remove job [" << id << "]";
            }
            emit jobIsDone(id);
            return;
        }
    }
    WARN() << "Wrong JobId = " << id;
}
