#pragma once

#include <sys/select.h>
#include <memory>
#include <QVector>
#include "curl/curl.h"

class ProtocolSlave;

class ProcessorSlave
{
public:
    explicit ProcessorSlave(int32_t id);
    virtual ~ProcessorSlave();
    int32_t id() const;
    int64_t timeout() const;
    void descriptors(fd_set *fdread, fd_set *fdwrite, fd_set *fdexcep, int32_t *maxfd) const;
    CURLM* getMultiHandle();
    bool add(ProtocolSlave *protocol);
    bool remove(ProtocolSlave *protocol);
    void execute();

private:
    ProcessorSlave();
    int32_t mId;
    CURLM  *mMh;
    int32_t mNowRunningProtocols;
    int32_t mLastRunningProtocols;
    QVector<ProtocolSlave*> mProtocols;
};
