#pragma once

#include <QObject>
#include <memory>
#include <vector>
#include "../HttpEnums.h"
#include "ProcessorSlave.h"

class ProtocolMaster;

/*! \brief class ProcessorMaster handles Libcurl multiHandle (high-level) */
class ProcessorMaster : public QObject
{
    Q_OBJECT
public:
    explicit ProcessorMaster(int32_t id);
    ~ProcessorMaster();
    ProcessorSlave *slave();
    void jobExecuted();

signals:
    void addProcessorToExecution(ProcessorMaster *processor);
    void removeProcessorFromExecution(ProcessorMaster *processor);
    void protocolProcessingFinished(qint32 id);

public slots:
    void addProtocolToProcessing(ProtocolMaster *protocol);
    void removeProtocolFromProcessing(ProtocolMaster *protocol);

private:
    ProcessorMaster();
    void addProtocolMasters();

    HttpError getErrorFromLibcurl();

    uint32_t       mId;
    bool           mActive;
    ProcessorSlave mProcessorSlave;

    QVector<ProtocolMaster *> mProtocolMasters;
    QVector<ProtocolMaster *> mProtocolMastersAdd;
};

