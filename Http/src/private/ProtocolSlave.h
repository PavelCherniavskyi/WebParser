/**
 * This file implements LibCurl EasyHandle handling
 * @file ProtocolSlave.h
 * @copyright (C) 2015 Harman Becker Automotive Systems GmbH
 *
 * @author Aleksandr Stankov  <astankov@luxoft.com>
 */

#pragma once

#include <QObject>

#include "curl/curl.h"

/*! \brief implementation of ProtocolSlave class */
class ProtocolSlave : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolSlave(const int32_t id);
    ~ProtocolSlave();
    int32_t     id() const;
    void        processExecutionResult(const CURLcode executionResult);
    void        setParams(const std::string &url, const QByteArray  &data);
    void        setVerboseLogging(const bool activate);
    bool        active() const;
    void        setActive(bool active);
    CURL*       easyHandle() const;
    CURLcode    result() const;
    uint16_t    httpResponseCode() const;

signals:

private:
    ProtocolSlave(); // default constructor is denied
    void            setTimeout(const uint32_t ms);
    static size_t   responseHeaderDispatcherCallback(void *ptr, size_t size, size_t nmemb, void *stream);
    size_t          responseHeaderCallback(void *ptr, size_t size, size_t nmemb);
    static size_t   writeDispatcherCallback(void *ptr, size_t size, size_t nmemb, void *stream);
    size_t          writeCallback(void *ptr, size_t size, size_t nmemb);
    static int      curlDebugDispatcherCallback(CURL *curl, curl_infotype infoType, char *text, size_t size, void *data);
    int32_t         curlDebugCallback(CURL *curl, curl_infotype infoType, char *text, size_t size);

    const int32_t            mId;
    CURL*                    mEasyHandle;
    bool                     mActive;
    CURLcode                 mResult;
};
