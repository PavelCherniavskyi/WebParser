#include "ProcessorSlave.h"
#include "../../../SmartLogger/src/SmartLogger.h"
#include "ProtocolSlave.h"

const int64_t defaultLibcurlSocketTimeoutMs = 1000;

//--------------------------------------------------------------------------------------------------
ProcessorSlave::ProcessorSlave(uint32_t id)
//--------------------------------------------------------------------------------------------------
    : mId(id)
    , mMh(nullptr)
    , mNowRunningProtocols(0)
    , mLastRunningProtocols(0)
{
    INFO() << "Id = " << id;
    mMh = curl_multi_init();
}

ProcessorSlave::~ProcessorSlave()
{
    INFO() << "[" << mId << "] destructor";
    curl_multi_cleanup(mMh);
    mMh = nullptr;
}

uint32_t ProcessorSlave::id() const
{
    return mId;
}

int64_t ProcessorSlave::timeout() const
{
    int64_t socketTimeoutMs = defaultLibcurlSocketTimeoutMs;

    if (CURLM_OK != curl_multi_timeout(mMh, &socketTimeoutMs)) {
        WARN() << "[" << mId << "] Cannot get socket timeout. Use default one(" << defaultLibcurlSocketTimeoutMs << "ms)";
        socketTimeoutMs = defaultLibcurlSocketTimeoutMs;
    }

    return socketTimeoutMs;
}

void ProcessorSlave::descriptors(fd_set *fdread, fd_set *fdwrite, fd_set *fdexcep, int32_t *maxfd) const
{
    if (CURLM_OK != curl_multi_fdset(mMh, fdread, fdwrite, fdexcep, maxfd)) {
        WARN() << "[" << mId << "] Cannot get socket descriptors!";
    }
}

CURLM *ProcessorSlave::getMultiHandle()
{
    return mMh;
}

bool ProcessorSlave::add(ProtocolSlave *protocol)
{
    bool success = false;

    if (nullptr != protocol) {
        INFO() << "Add protocol[" << protocol->id() << "]";
        mProtocols.push_back(protocol);
        protocol->setActive(true);

        if(CURLM_OK == curl_multi_add_handle(mMh, protocol->easyHandle())) {
            mLastRunningProtocols++;
            success = true;
        } else {
            WARN() << "[" << mId << "] Cannot add easyHandle to multiHandle!";
        }
    } else {
        WARN() << "[" << mId << "] nullptr == protocol!";
    }
    return success;
}

bool ProcessorSlave::remove(ProtocolSlave *protocol)
{
    INFO();
    bool success = true;

    if (nullptr != protocol) {
        protocol->setActive(false);

        auto iter = std::find(mProtocols.begin(), mProtocols.end(), protocol);

        if (mProtocols.end() != iter) {
            mProtocols.erase(iter);
        } else {
            WARN() << "[" << mId << "] Cannot find protocol!";
            success = false;
        }

        if (CURLM_OK != curl_multi_remove_handle(mMh, protocol->easyHandle())) {
            WARN() << "[" << mId << "] Cannot remove easyHandle from multiHandle";
            success = false;
        }
    }

    return success;
}

void ProcessorSlave::execute()
{
    while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(mMh, &mNowRunningProtocols)) {
    }
    if (mNowRunningProtocols < mLastRunningProtocols) {
        mLastRunningProtocols = mNowRunningProtocols;

        CURLMsg *msg;
        int32_t  msgsLeft;

        while (nullptr != (msg = curl_multi_info_read(mMh, &msgsLeft))) {
            if (CURLMSG_DONE == msg->msg) {
                auto iter = std::find_if(mProtocols.begin(), mProtocols.end()
                            , [msg](ProtocolSlave *protocol)
                              {return protocol->easyHandle() == msg->easy_handle; });

                if (mProtocols.end() != iter) {
                    (*iter)->processExecutionResult(msg->data.result);
                } else {
                    WARN() << "[" << mId << "] Cannot find easyHandle for completed transfer!";
                }
            }
        }
    }
}
