#include "../../../SmartLogger/src/SmartLogger.h"
#include "../Job.h"

Job::Job(QObject *obj) : QObject(obj)
    , mId(-1)
    , mPrintDebugInfo(false)
{
    connect(this, &Job::continueExecution, this, &Job::executeJob, Qt::QueuedConnection);
}

Job::~Job()
{
    // do nothing
}

void Job::setId(int32_t id)
{
    mId = id;
}

int32_t Job::id() const
{
    return mId;
}

void Job::executeJob()
{
    if(mPrintDebugInfo){
        INFO() << "Job[" << mId << "]: calling execute()";
    }

    bool continueExec = execute();

    if(mPrintDebugInfo){
        INFO() << "Job[" << mId << "]: execute() call finished";
    }

    if (continueExec) {
        emit continueExecution();
    } else {
        emit executionCompleted(mId);
    }
}
