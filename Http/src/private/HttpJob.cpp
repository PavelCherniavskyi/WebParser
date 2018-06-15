#include "HttpJob.h"

#include "../../../SmartLogger/src/SmartLogger.h"
#include "ProcessorSlave.h"
#include <QThread>

HttpJob::HttpJob(const QVector<ProcessorSlave *> &processorSlaves) : mProcessorSlaves(processorSlaves)
{
}

HttpJob::~HttpJob()
{
}

bool HttpJob::execute()
{
    INFO() << "mProcessorSlaves size: " << mProcessorSlaves.size();
    curlSelect();

    for (auto &processor : mProcessorSlaves) {
        processor->execute();
    }

    emit jobExecuted();

    return false;
}

int32_t HttpJob::id() const
{
    return mId;
}

void HttpJob::setId(int32_t id)
{
    mId = id;
}

void HttpJob::curlSelect()
{
    INFO() << "mProcessorSlaves size: " << mProcessorSlaves.size();

    const int64_t defaultTimeoutMs = 200;

    int64_t timeoutMs = defaultTimeoutMs;
    struct timeval selectTimeout;

    fd_set fdRead;
    fd_set fdWrite;
    fd_set fdExcep;
    int maxFd = -1;

    // initialize file descriptor sets
    FD_ZERO(&fdRead);
    FD_ZERO(&fdWrite);
    FD_ZERO(&fdExcep);

    for (auto &processor : mProcessorSlaves) {
        // get file descriptors
        int maxFdLocal = -1;
        processor->descriptors(&fdRead, &fdWrite, &fdExcep, &maxFd);
        maxFd = std::max(maxFd, maxFdLocal);

        // get timeout
        int64_t timeoutMsLocal = processor->timeout();
        timeoutMs = std::min(timeoutMs, timeoutMsLocal);
    }

    // check whether the select timeout returned by libCurl is valid
    if (-1 == timeoutMs) {
        // libCurl currently has no timeout stored, use the default one
        timeoutMs = defaultTimeoutMs;
    } else {
        // libCurl timeout is valid, ensure that its value is
        // LIBCURL_SOCKET_TIMEOUT_DEFAULT_MS at most. Otherwise
        // the thread may block until the timeout expired and
        // thereby reduce the responsiveness to events received
        // via its input queue.
        timeoutMs = std::min(timeoutMs, defaultTimeoutMs);
    }

    INFO() << "maxFd: " << maxFd;
    if (timeoutMs) {
        if (-1 == maxFd) {
            // no file descriptors are set by libCurl, sleep for the select timeout
            // returned by libCurl or the default one
            QThread::msleep(timeoutMs);
        } else {
            // there are file descriptors set by libCurl, select on them
            selectTimeout.tv_sec = timeoutMs / 1000;
            selectTimeout.tv_usec = (timeoutMs % 1000 ) * 1000;

            int rc = select(maxFd + 1, &fdRead, &fdWrite, &fdExcep, &selectTimeout);

            if (-1 == rc) {
                INFO() << "select error " << errno;
            }
        }
    }
}
