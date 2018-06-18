#include "../../../SmartLogger/src/SmartLogger.h"
#include "../Job.h"
#include <QCoreApplication>

Job::Job(QObject *obj) : QObject(obj)
    , mId(-1)
    , mPrintDebugInfo(false)
{
    //connect(this, &Job::continueExecution, this, &Job::executeJob, Qt::QuedConnection);
}

Job::~Job()
{
    // do nothing
}

void Job::setId(int id)
{
    mId = id;
}

int Job::id() const
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
        executeJob();
    } else {
        emit executionCompleted(mId);
    }
}
