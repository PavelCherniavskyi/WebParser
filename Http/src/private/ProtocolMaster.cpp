#include "ProtocolMaster.h"
#include "../../../SmartLogger/src/SmartLogger.h"

ProtocolMaster::ProtocolMaster(QSharedPointer<IRequestSender> sender, int32_t id)
    : mId(id)
    , mProtocolSlave(id)
    , mRequestSender(sender)
    , mAbortRequested(false)
{
    INFO() << "[" << mId << "] constructor";
}

//--------------------------------------------------------------------------------------------------
ProtocolMaster::~ProtocolMaster()
//--------------------------------------------------------------------------------------------------
{
    INFO() << "[" << mId << "] destructor";

    if (mProtocolSlave.active()) {
        emit removeProtocolFromProcessing(this);
    }
}

//--------------------------------------------------------------------------------------------------
int32_t ProtocolMaster::id() const
//--------------------------------------------------------------------------------------------------
{
    return mId;
}

//--------------------------------------------------------------------------------------------------
ProtocolSlave *ProtocolMaster::slave()
//--------------------------------------------------------------------------------------------------
{
    return &mProtocolSlave;
}

//--------------------------------------------------------------------------------------------------
void ProtocolMaster::abort()
//--------------------------------------------------------------------------------------------------
{
    mAbortRequested = true;
}

//--------------------------------------------------------------------------------------------------
bool ProtocolMaster::sendRequest(const QString &url, const QByteArray  &data)
//--------------------------------------------------------------------------------------------------
{
    bool success = false;

    INFO() << "[" << mId << "] sendRequest(" << url << " " << data.size() << "bytes)";

    if (!mProtocolSlave.active()) {
        mProtocolSlave.setParams(url, data);

        emit addProtocolToProcessing(this);
        success = true;
    } else {
        WARN() << "[" << mId << "] Cannot send request because easyHandle is already active!";
    }

    return success;
}

//--------------------------------------------------------------------------------------------------
HttpError ProtocolMaster::getErrorFromLibcurl()
//--------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------
void ProtocolMaster::jobExecuted()
//--------------------------------------------------------------------------------------------------
{
    if (!mProtocolSlave.active()) {
        HttpError error = getErrorFromLibcurl();

//        const QByteArray data = mProtocolSlave.responseData();

        INFO() << "[" << mId << "] send informationResponseInfoValue( error = " << static_cast<int>(error) << ", " << "Empty" /*data.size()*/ << "bytes )";

//        mHttpService->informationResponseInfoValue(ResponseInfoParams(mId,
//                                                                    mProtocolSlave.httpResponseCode(),
//                                                                    mProtocolSlave.responseHeaders(),
//                                                                    mProtocolSlave.cookies()));

//        mHttpService->informationResponseDataValue(ResponseDataParams(mId, std::vector<uint8_t>(data.data(), data.data() + data.size())));

//        mHttpService->informationProcessingFinishedValue(ProcessingFinishedParams(mId, getErrorFromLibcurl()));

        emit removeProtocolFromProcessing(this);
    }
}
