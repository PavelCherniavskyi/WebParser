#pragma once

#include <QObject>
#include <QSharedPointer>
#include <memory>
#include <vector>

#include "HttpJob.h"

class ProcessorMaster;
class JobExecutor;

/*! \brief class ProcessorExecutor handles execution of multiple libcurl multihandles */
class ProcessorExecutor : public QObject
{
    Q_OBJECT
public:
    explicit ProcessorExecutor();
    virtual ~ProcessorExecutor();
    void init();
    void deinit();
    bool isExecuting();


signals:
    void executeJob(const QVector<ProcessorSlave *>& processorSlaves);

public slots:
    void addProcessorToExecution(ProcessorMaster *processor);
    void removeProcessorFromExecution(ProcessorMaster *processor);
    void handleJobExecuted();

private:
    bool executeProcessorSlaves();

    bool                           mActive;
    std::unique_ptr<JobExecutor>   mHttpJobExecutor;
    std::vector<ProcessorMaster*> mProcessorMasters;
};

