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
    /*! \brief constructor */
    explicit ProcessorExecutor();

    /*! \brief destructor */
    virtual ~ProcessorExecutor();

    /*! \brief performs init */
    void init();

    /*! \brief performs deinit */
    void deinit();

    /*! \brief returns ProcessorExecutor */
    bool isExecuting();


signals:
    /*! sends event to execute job in worker thread */
    void executeJob(const std::vector<ProcessorSlave *>& processorSlaves);

public slots:
    /*! \brief adds processor to execution queue */
    void addProcessorToExecution(ProcessorMaster *processor);

    /*! \brief removes processor from execution queue */
    void removeProcessorFromExecution(ProcessorMaster *processor);

    /*! \brief handles synchronisation event from HttpJob */
    void handleJobExecuted();

private:
    /*! \brief executes processorSlaves */
    bool executeProcessorSlaves();

    bool                           mActive;
    std::unique_ptr<JobExecutor>   mHttpJobExecutor;
    std::vector<ProcessorMaster*> mProcessorMasters;
};

