#include "ProcessorExecutor.h"
#include "../../../Utils/src/JobExecutor.h"
#include "../../../SmartLogger/src/SmartLogger.h"
#include "ProcessorMaster.h"
#include "ProcessorSlave.h"

ProcessorExecutor::ProcessorExecutor(QObject *obj) : IProcessorExecutor(obj)
    , mActive(false)
    , mHttpJobExecutor(new JobExecutor)
{
    INFO() << "constructor";
}

ProcessorExecutor::~ProcessorExecutor()
{
    INFO() << "destructor";
}

//--------------------------------------------------------------------------------------------------
bool ProcessorExecutor::isExecuting()
//--------------------------------------------------------------------------------------------------
{
    return mActive;
}

//--------------------------------------------------------------------------------------------------
bool ProcessorExecutor::executeProcessorSlaves()
//--------------------------------------------------------------------------------------------------
{
    bool executing = false;

    QVector<ProcessorSlave *> processorSlaves;

    if (!mProcessorMasters.empty()) {
        for(auto processorMaster : mProcessorMasters) {
            processorSlaves.push_back(processorMaster->slave());
        }

        QSharedPointer<HttpJob> job(new HttpJob(processorSlaves), &QObject::deleteLater);
        connect(job.data(), &HttpJob::jobExecuted, this, &ProcessorExecutor::handleJobExecuted);
        mHttpJobExecutor->execute(job, false);

        executing = true;
    }

    return executing;
}

//--------------------------------------------------------------------------------------------------
void ProcessorExecutor::addProcessorToExecution(ProcessorMaster *processor)
//--------------------------------------------------------------------------------------------------
{
    if (processor) {
        mProcessorMasters.push_back(processor);

        if (!mActive) {
            mActive = executeProcessorSlaves();
            if (mActive) {
                INFO() << "start execution! " << mProcessorMasters.size() << " processors";
            }
        } else {
             // Do nothing. Wait until execution is completed
        }
    } else {
        WARN() << "Processor is null!";
    }
}

//--------------------------------------------------------------------------------------------------
void ProcessorExecutor::removeProcessorFromExecution(ProcessorMaster *processor)
//--------------------------------------------------------------------------------------------------
{
    if (processor) {
        auto iter = std::find(mProcessorMasters.begin(), mProcessorMasters.end(), processor);

        if (mProcessorMasters.end() != iter) {
            mProcessorMasters.erase(iter);
        } else {
            WARN() << "Cannot find processor!";
        }
    } else {
        WARN() << "Processor is null!";
    }
}

//--------------------------------------------------------------------------------------------------
void ProcessorExecutor::handleJobExecuted()
//--------------------------------------------------------------------------------------------------
{
    if (mActive) {
        for (auto &processorMaster : mProcessorMasters) {
            processorMaster->jobExecuted();
        }

        mActive = executeProcessorSlaves();
    }
}
