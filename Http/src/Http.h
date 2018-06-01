#pragma once

#include <QObject>
#include "private/LibCurlConfig.h"
#include "private/ProtocolMaster.h"
#include "private/ProcessorMaster.h"
#include "private/ProcessorExecutor.h"

class Provisioning;
class ProcessorExecutor;
class IRequestSender;

class Http : public QObject
{
    Q_OBJECT
public:
    struct ProvData {
        QList<QString> urls;
        uint timeout;
    };

    Http(QObject *obj);
    ~Http();
    void init(Provisioning* prov);
    void sendRequest(QSharedPointer<IRequestSender> sender, const QString& url, const QVector<uint8_t>& data);

private slots:
    void OnProvDataReceived(Http::ProvData provData);

private:
    LibCurlConfig                               mLibCurlConfig;
    QVector<QSharedPointer<ProtocolMaster>>     mProtocolMasters;
    QSharedPointer<ProcessorMaster>             mProcessorMaster;
    ProcessorExecutor                           mProcessorExecutor;
    int32_t                                     mProtocolIdGenerator; // generator for unique IDs
    ProvData                                    mProvData;
};
