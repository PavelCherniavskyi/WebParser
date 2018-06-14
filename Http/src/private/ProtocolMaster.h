#pragma once

#include <QObject>
#include <memory>
#include <vector>
#include <QSharedPointer>
#include "ProtocolSlave.h"
#include "../HttpEnums.h"
#include "../HttpTypes.h"

class Http;
class IDownloadManager;

class ProtocolMaster : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolMaster(IDownloadManager *sender, int32_t id, QObject *obj = 0);
    virtual ~ProtocolMaster();
    int32_t id() const;
    ProtocolSlave *slave();
    bool sendRequest(const QString &url, int port = 80);

signals:
    void addProtocolToProcessing(ProtocolMaster *protocol);
    void removeProtocolFromProcessing(ProtocolMaster *protocol);

public slots:
    void jobExecuted();

private:
    ProtocolMaster();
    HttpError getErrorFromLibcurl();

    int32_t       mId;
    ProtocolSlave mProtocolSlave;

    IDownloadManager *mRequestSender; // this is necessary for sending responses:
};

