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
class ProtocolMaster;

class IProtocolMaster : public QObject
{
    Q_OBJECT
public:
    IProtocolMaster(QObject *obj = nullptr) : QObject(obj){}
    virtual int32_t id() const = 0;
    virtual bool sendRequest(const QString &url) = 0;
    virtual ProtocolSlave *slave() = 0;

signals:
    void addProtocolToProcessing(ProtocolMaster *protocol);
    void removeProtocolFromProcessing(ProtocolMaster *protocol);

public slots:
    virtual void jobExecuted() = 0;
};

class ProtocolMaster : public IProtocolMaster
{
    Q_OBJECT
public:
    explicit ProtocolMaster(IDownloadManager *sender, int32_t id, QObject *obj = nullptr);
    virtual ~ProtocolMaster() override;
    int32_t id() const override;
    ProtocolSlave *slave() override;
    bool sendRequest(const QString &url) override;

public slots:
    void jobExecuted() override;

private:
    ProtocolMaster();
    HttpError getErrorFromLibcurl();

    int32_t       mId;
    ProtocolSlave mProtocolSlave;
    ResponseDataParams responseParams;
    IDownloadManager *mRequestSender; // this is necessary for sending responses:
};

