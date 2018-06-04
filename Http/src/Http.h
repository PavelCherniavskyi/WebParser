#pragma once

#include <QObject>
#include "private/LibCurlConfig.h"
#include "private/ProtocolMaster.h"
#include "private/ProcessorMaster.h"
#include "private/ProcessorExecutor.h"

class Provisioning;
class ProcessorExecutor;
class ResponseHandler;
class DownloadManager;

class Http : public QObject
{
    Q_OBJECT
public:
    Http(QObject *obj);
    ~Http();
    void init(Provisioning *prov);
    void sendRequest(DownloadManager *sender, const QString& url);

private slots:
    void protocolProcessingFinished(qint32 id);

private:
    LibCurlConfig                               mLibCurlConfig;
    QVector<QSharedPointer<ProtocolMaster>>     mProtocolMasters;
    QSharedPointer<ProcessorMaster>             mProcessorMaster;
    ProcessorExecutor                           mProcessorExecutor;
    int32_t                                     mProtocolIdGenerator; // generator for unique IDs
};
