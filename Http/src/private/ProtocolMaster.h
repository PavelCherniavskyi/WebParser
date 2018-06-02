#pragma once

#include <QObject>
#include <memory>
#include <vector>
#include <QSharedPointer>
#include "ProtocolSlave.h"
#include "../HttpEnums.h"
#include "../HttpTypes.h"
#include "../../../../WebParser/IRequestSender.h"

class Http;

/*! \brief class ProtocolMaster handles Libcurl easyHandle (high-level) */
class ProtocolMaster : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolMaster(QSharedPointer<IRequestSender> sender, int32_t id);
    virtual ~ProtocolMaster();
    int32_t id() const;
    ProtocolSlave *slave();
    void abort();
    bool sendRequest(const QString &url);

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

    QSharedPointer<IRequestSender> mRequestSender; // this is necessary for sending responses:

    bool          mAbortRequested;
};

