#pragma once

#include <QObject>
#include <memory>
#include <vector>
#include "../HttpEnums.h"
#include "ProcessorSlave.h"

class IProtocolMaster;
class ProcessorMaster;

class IProcessorMaster : public QObject
{
    Q_OBJECT
public:
    IProcessorMaster(QObject *obj = nullptr) : QObject(obj) {}
    virtual void jobExecuted() = 0;

signals:
    void addProcessorToExecution(ProcessorMaster *processor);
    void removeProcessorFromExecution(ProcessorMaster *processor);
    void protocolProcessingFinished(qint32 id);

public slots:
    virtual void addProtocolToProcessing(IProtocolMaster *protocol) = 0;
    virtual void removeProtocolFromProcessing(IProtocolMaster *protocol) = 0;
};

class ProcessorMaster : public IProcessorMaster
{
    Q_OBJECT
public:
    explicit ProcessorMaster(uint32_t id, QObject *obj = nullptr);
    ~ProcessorMaster() override;
    ProcessorSlave *slave();
    void jobExecuted() override;
    uint32_t isActive();
    uint32_t Id();

public slots:
    void addProtocolToProcessing(IProtocolMaster *protocol) override;
    void removeProtocolFromProcessing(IProtocolMaster *protocol) override;

private:
    ProcessorMaster();
    void addProtocolMasters();
    HttpError getErrorFromLibcurl();

    uint32_t       mId;
    bool           mActive;
    QVector<IProtocolMaster *> mProtocolMastersAdd;
    ProcessorSlave mProcessorSlave;
    QVector<IProtocolMaster *> mProtocolMasters;
};

