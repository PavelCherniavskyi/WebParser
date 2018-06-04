#include "../../../SmartLogger/src/SmartLogger.h"
#include "../Job.h"

Job::Job(const QString &name) : QObject()
    , mId(-1)
    , mName(name)
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

const QString &Job::name()
{
    return mName;
}

void Job::setPrintDebugInfo(bool printDebugInfo)
{
    mPrintDebugInfo = printDebugInfo;
}

void Job::executeJob()
{
    if(mPrintDebugInfo){
        INFO() << "Job[" << mId << "]:'" << mName << "' calling execute()";
    }

    bool continueExec = execute();

    if(mPrintDebugInfo){
        INFO() << "Job[" << mId << "]:'" << mName << "' execute() call finished";
    }

    if (continueExec) {
        emit continueExecution();
    } else {
        emit executionCompleted(mId);
    }
}
