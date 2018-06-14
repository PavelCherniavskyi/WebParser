#pragma once

#include <QObject>
#include <QSharedPointer>
#include <memory>
#include <vector>

#include "HttpJob.h"

class ProcessorMaster;
class JobExecutor;


class IProcessorExecutor : public QObject
{
    Q_OBJECT
public:
    IProcessorExecutor(QObject *obj = 0) : QObject(obj){}
signals:
    void executeJob(const QVector<ProcessorSlave *> &processorSlaves);

public slots:
    virtual void addProcessorToExecution(ProcessorMaster *processor) = 0;
    virtual void removeProcessorFromExecution(ProcessorMaster *processor) = 0;
    virtual void handleJobExecuted() = 0;
};

class ProcessorExecutor : public IProcessorExecutor
{
    Q_OBJECT
public:
    explicit ProcessorExecutor(QObject *obj = 0);
    virtual ~ProcessorExecutor();
    bool isExecuting();

public slots:
    void addProcessorToExecution(ProcessorMaster *processor) override;
    void removeProcessorFromExecution(ProcessorMaster *processor) override;
    void handleJobExecuted() override;

private:
    bool executeProcessorSlaves();

    bool                           mActive;
    std::unique_ptr<JobExecutor>   mHttpJobExecutor;
    std::vector<ProcessorMaster*> mProcessorMasters;
};

