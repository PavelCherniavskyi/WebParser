#include "ProtocolMaster.h"
#include "../../../SmartLogger/src/SmartLogger.h"
#include "../../../DownloadManager/src/DownloadManager.h"

ProtocolMaster::ProtocolMaster(IDownloadManager *sender, int32_t id, QObject *obj) : IProtocolMaster(obj)
    , mId(id)
    , mProtocolSlave(id)
    , mRequestSender(sender)
{
    INFO() << "[" << mId << "] constructor";
    if(mRequestSender == nullptr) {
        ERROR() << "Request sender is empty";
    }
}

ProtocolMaster::~ProtocolMaster()
{
    INFO() << "[" << mId << "] destructor";

    if (mProtocolSlave.active()) {
        emit removeProtocolFromProcessing(this);
    }
}

int32_t ProtocolMaster::id() const
{
    return mId;
}

ProtocolSlave *ProtocolMaster::slave()
{
    return &mProtocolSlave;
}

bool ProtocolMaster::sendRequest(const QString &url)
{
    bool success = false;
    responseParams.url = url;
    INFO() << "[" << mId << "] url: " << url;

    if (!mProtocolSlave.active()) {
        mProtocolSlave.setUrl(url);
        //mProtocolSlave.setPort(port);

        emit addProtocolToProcessing(this);
        success = true;
    } else {
        WARN() << "[" << mId << "] Cannot send request because easyHandle is already active!";
    }

    return success;
}

HttpError ProtocolMaster::getErrorFromLibcurl()
{
    HttpError error = HttpError::NO_ERROR;

    switch (mProtocolSlave.result()) {
    case CURLE_OK:
        error = HttpError::NO_ERROR;
        break;

    case CURLE_OPERATION_TIMEDOUT:  // fall through
    case CURLE_COULDNT_CONNECT:     // the connection error occurred
        error = HttpError::TRANSFER_OPERATION_TIMEOUT;
        break;

    case CURLE_SSL_CONNECT_ERROR:        // fall through all SSL/TLS error codes
    case CURLE_PEER_FAILED_VERIFICATION:
    case CURLE_SSL_ENGINE_NOTFOUND:
    case CURLE_SSL_ENGINE_SETFAILED:
    case CURLE_SSL_CERTPROBLEM:
    case CURLE_SSL_CIPHER:
    case CURLE_SSL_CACERT:
    case CURLE_USE_SSL_FAILED:
    case CURLE_SSL_ENGINE_INITFAILED:
    case CURLE_SSL_CACERT_BADFILE:
    case CURLE_SSL_SHUTDOWN_FAILED:
    case CURLE_SSL_CRL_BADFILE:
    case CURLE_SSL_ISSUER_ERROR:
    case CURLE_SSL_PINNEDPUBKEYNOTMATCH:
    case CURLE_SSL_INVALIDCERTSTATUS:
        error = HttpError::ERROR_TLS;
        break;

    default:
        error = HttpError::ERROR_GENERIC;
        break;
    }

    return error;
}

void ProtocolMaster::jobExecuted()
{
    if (!mProtocolSlave.active()) {
        responseParams.id = mId;
        responseParams.header = mProtocolSlave.responseHeader();
        responseParams.data = mProtocolSlave.responseData();
        responseParams.error = getErrorFromLibcurl();

        mRequestSender->setResponseData(responseParams);

        emit removeProtocolFromProcessing(this);
    }
}
