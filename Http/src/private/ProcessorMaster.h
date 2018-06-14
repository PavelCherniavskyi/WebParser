#pragma once

#include <QObject>
#include <memory>
#include <vector>
#include "../HttpEnums.h"
#include "ProcessorSlave.h"

class ProtocolMaster;
class ProcessorMaster;

class IProcessorMaster : public QObject
{
    Q_OBJECT
public:
    IProcessorMaster(QObject *obj = 0) : QObject(obj) {}
    virtual void jobExecuted() = 0;

signals:
    void addProcessorToExecution(ProcessorMaster *processor);
    void removeProcessorFromExecution(ProcessorMaster *processor);
    void protocolProcessingFinished(qint32 id);

public slots:
    virtual void addProtocolToProcessing(ProtocolMaster *protocol) = 0;
    virtual void removeProtocolFromProcessing(ProtocolMaster *protocol) = 0;
};

class ProcessorMaster : public IProcessorMaster
{
    Q_OBJECT
public:
    explicit ProcessorMaster(int32_t id, QObject *obj = 0);
    ~ProcessorMaster();
    ProcessorSlave *slave();
    void jobExecuted() override;

public slots:
    void addProtocolToProcessing(ProtocolMaster *protocol) override;
    void removeProtocolFromProcessing(ProtocolMaster *protocol) override;

private:
    ProcessorMaster();
    void addProtocolMasters();
    HttpError getErrorFromLibcurl();

    uint32_t       mId;
    bool           mActive;
    QVector<ProtocolMaster *> mProtocolMastersAdd;
    ProcessorSlave mProcessorSlave;
    QVector<ProtocolMaster *> mProtocolMasters;
};

