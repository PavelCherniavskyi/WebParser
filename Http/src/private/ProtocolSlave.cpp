#include <QMap>
#include "curl/curl.h"
#include "ProtocolSlave.h"
#include "../../../SmartLogger/src/SmartLogger.h"

const uint32_t DEFAULT_OPERATION_TIMEOUT_MS = 30000;

const QMap<CURLcode, QString> curlCodeToText = {
    {CURLE_OK,                       "CURLE_OK"                      },
    {CURLE_UNSUPPORTED_PROTOCOL,     "CURLE_UNSUPPORTED_PROTOCOL"    },
    {CURLE_FAILED_INIT,              "CURLE_FAILED_INIT"             },
    {CURLE_URL_MALFORMAT,            "CURLE_URL_MALFORMAT"           },
    {CURLE_NOT_BUILT_IN,             "CURLE_NOT_BUILT_IN"            },
    {CURLE_COULDNT_RESOLVE_PROXY,    "CURLE_COULDNT_RESOLVE_PROXY"   },
    {CURLE_COULDNT_RESOLVE_HOST,     "CURLE_COULDNT_RESOLVE_HOST"    },
    {CURLE_COULDNT_CONNECT,          "CURLE_COULDNT_CONNECT"         },
    {CURLE_FTP_WEIRD_SERVER_REPLY,   "CURLE_FTP_WEIRD_SERVER_REPLY"  },
    {CURLE_REMOTE_ACCESS_DENIED,     "CURLE_REMOTE_ACCESS_DENIED"    },
    {CURLE_FTP_ACCEPT_FAILED,        "CURLE_FTP_ACCEPT_FAILED"       },
    {CURLE_FTP_WEIRD_PASS_REPLY,     "CURLE_FTP_WEIRD_PASS_REPLY"    },
    {CURLE_FTP_ACCEPT_TIMEOUT,       "CURLE_FTP_ACCEPT_TIMEOUT"      },
    {CURLE_FTP_WEIRD_PASV_REPLY,     "CURLE_FTP_WEIRD_PASV_REPLY"    },
    {CURLE_FTP_WEIRD_227_FORMAT,     "CURLE_FTP_WEIRD_227_FORMAT"    },
    {CURLE_FTP_CANT_GET_HOST,        "CURLE_FTP_CANT_GET_HOST"       },
    {CURLE_HTTP2,                    "CURLE_HTTP2"                   },
    {CURLE_FTP_COULDNT_SET_TYPE,     "CURLE_FTP_COULDNT_SET_TYPE"    },
    {CURLE_PARTIAL_FILE,             "CURLE_PARTIAL_FILE"            },
    {CURLE_FTP_COULDNT_RETR_FILE,    "CURLE_FTP_COULDNT_RETR_FILE"   },
    {CURLE_OBSOLETE20,               "CURLE_OBSOLETE20"              },
    {CURLE_QUOTE_ERROR,              "CURLE_QUOTE_ERROR"             },
    {CURLE_HTTP_RETURNED_ERROR,      "CURLE_HTTP_RETURNED_ERROR"     },
    {CURLE_WRITE_ERROR,              "CURLE_WRITE_ERROR"             },
    {CURLE_OBSOLETE24,               "CURLE_OBSOLETE24"              },
    {CURLE_UPLOAD_FAILED,            "CURLE_UPLOAD_FAILED"           },
    {CURLE_READ_ERROR,               "CURLE_READ_ERROR"              },
    {CURLE_OUT_OF_MEMORY,            "CURLE_OUT_OF_MEMORY"           },
    {CURLE_OPERATION_TIMEDOUT,       "CURLE_OPERATION_TIMEDOUT"      },
    {CURLE_OBSOLETE29,               "CURLE_OBSOLETE29"              },
    {CURLE_FTP_PORT_FAILED,          "CURLE_FTP_PORT_FAILED"         },
    {CURLE_FTP_COULDNT_USE_REST,     "CURLE_FTP_COULDNT_USE_REST"    },
    {CURLE_OBSOLETE32,               "CURLE_OBSOLETE32"              },
    {CURLE_RANGE_ERROR,              "CURLE_RANGE_ERROR"             },
    {CURLE_HTTP_POST_ERROR,          "CURLE_HTTP_POST_ERROR"         },
    {CURLE_SSL_CONNECT_ERROR,        "CURLE_SSL_CONNECT_ERROR"       },
    {CURLE_BAD_DOWNLOAD_RESUME,      "CURLE_BAD_DOWNLOAD_RESUME"     },
    {CURLE_FILE_COULDNT_READ_FILE,   "CURLE_FILE_COULDNT_READ_FILE"  },
    {CURLE_LDAP_CANNOT_BIND,         "CURLE_LDAP_CANNOT_BIND"        },
    {CURLE_LDAP_SEARCH_FAILED,       "CURLE_LDAP_SEARCH_FAILED"      },
    {CURLE_OBSOLETE40,               "CURLE_OBSOLETE40"              },
    {CURLE_FUNCTION_NOT_FOUND,       "CURLE_FUNCTION_NOT_FOUND"      },
    {CURLE_ABORTED_BY_CALLBACK,      "CURLE_ABORTED_BY_CALLBACK"     },
    {CURLE_BAD_FUNCTION_ARGUMENT,    "CURLE_BAD_FUNCTION_ARGUMENT"   },
    {CURLE_OBSOLETE44,               "CURLE_OBSOLETE44"              },
    {CURLE_INTERFACE_FAILED,         "CURLE_INTERFACE_FAILED"        },
    {CURLE_OBSOLETE46,               "CURLE_OBSOLETE46"              },
    {CURLE_TOO_MANY_REDIRECTS ,      "CURLE_TOO_MANY_REDIRECTS"      },
    {CURLE_UNKNOWN_OPTION,           "CURLE_UNKNOWN_OPTION"          },
    {CURLE_TELNET_OPTION_SYNTAX ,    "CURLE_TELNET_OPTION_SYNTAX"    },
    {CURLE_OBSOLETE50,               "CURLE_OBSOLETE50"              },
    {CURLE_PEER_FAILED_VERIFICATION, "CURLE_PEER_FAILED_VERIFICATION"},
    {CURLE_GOT_NOTHING,              "CURLE_GOT_NOTHING"             },
    {CURLE_SSL_ENGINE_NOTFOUND,      "CURLE_SSL_ENGINE_NOTFOUND"     },
    {CURLE_SSL_ENGINE_SETFAILED,     "CURLE_SSL_ENGINE_SETFAILED"    },
    {CURLE_SEND_ERROR,               "CURLE_SEND_ERROR"              },
    {CURLE_RECV_ERROR,               "CURLE_RECV_ERROR"              },
    {CURLE_OBSOLETE57,               "CURLE_OBSOLETE57"              },
    {CURLE_SSL_CERTPROBLEM,          "CURLE_SSL_CERTPROBLEM"         },
    {CURLE_SSL_CIPHER,               "CURLE_SSL_CIPHER"              },
    {CURLE_SSL_CACERT,               "CURLE_SSL_CACERT"              },
    {CURLE_BAD_CONTENT_ENCODING,     "CURLE_BAD_CONTENT_ENCODING"    },
    {CURLE_LDAP_INVALID_URL,         "CURLE_LDAP_INVALID_URL"        },
    {CURLE_FILESIZE_EXCEEDED,        "CURLE_FILESIZE_EXCEEDED"       },
    {CURLE_USE_SSL_FAILED,           "CURLE_USE_SSL_FAILED"          },
    {CURLE_SEND_FAIL_REWIND,         "CURLE_SEND_FAIL_REWIND"        },
    {CURLE_SSL_ENGINE_INITFAILED,    "CURLE_SSL_ENGINE_INITFAILED"   },
    {CURLE_LOGIN_DENIED,             "CURLE_LOGIN_DENIED"            },
    {CURLE_TFTP_NOTFOUND,            "CURLE_TFTP_NOTFOUND"           },
    {CURLE_TFTP_PERM,                "CURLE_TFTP_PERM"               },
    {CURLE_REMOTE_DISK_FULL,         "CURLE_REMOTE_DISK_FULL"        },
    {CURLE_TFTP_ILLEGAL,             "CURLE_TFTP_ILLEGAL"            },
    {CURLE_TFTP_UNKNOWNID,           "CURLE_TFTP_UNKNOWNID"          },
    {CURLE_REMOTE_FILE_EXISTS,       "CURLE_REMOTE_FILE_EXISTS"      },
    {CURLE_TFTP_NOSUCHUSER,          "CURLE_TFTP_NOSUCHUSER"         },
    {CURLE_CONV_FAILED,              "CURLE_CONV_FAILED"             },
    {CURLE_CONV_REQD,                "CURLE_CONV_REQD"               },
    {CURLE_SSL_CACERT_BADFILE,       "CURLE_SSL_CACERT_BADFILE"      },
    {CURLE_REMOTE_FILE_NOT_FOUND,    "CURLE_REMOTE_FILE_NOT_FOUND"   },
    {CURLE_SSH,                      "CURLE_SSH"                     },
    {CURLE_SSL_SHUTDOWN_FAILED,      "CURLE_SSL_SHUTDOWN_FAILED"     },
    {CURLE_AGAIN,                    "CURLE_AGAIN"                   },
    {CURLE_SSL_CRL_BADFILE,          "CURLE_SSL_CRL_BADFILE"         },
    {CURLE_SSL_ISSUER_ERROR,         "CURLE_SSL_ISSUER_ERROR"        },
    {CURLE_FTP_PRET_FAILED,          "CURLE_FTP_PRET_FAILED"         },
    {CURLE_RTSP_CSEQ_ERROR,          "CURLE_RTSP_CSEQ_ERROR"         },
    {CURLE_RTSP_SESSION_ERROR,       "CURLE_RTSP_SESSION_ERROR"      },
    {CURLE_FTP_BAD_FILE_LIST,        "CURLE_FTP_BAD_FILE_LIST"       },
    {CURLE_CHUNK_FAILED,             "CURLE_CHUNK_FAILED"            },
    {CURLE_NO_CONNECTION_AVAILABLE,  "CURLE_NO_CONNECTION_AVAILABLE" },
    {CURLE_SSL_PINNEDPUBKEYNOTMATCH, "CURLE_SSL_PINNEDPUBKEYNOTMATCH"},
    {CURLE_SSL_INVALIDCERTSTATUS,    "CURLE_SSL_INVALIDCERTSTATUS"   },
    {CURL_LAST,                      "CURL_LAST"                     }
};

//--------------------------------------------------------------------------------------------------
ProtocolSlave::ProtocolSlave(const int32_t id)
//--------------------------------------------------------------------------------------------------
    : mId(id)
    , mEasyHandle(0)
    , mActive(false)
    , mResult(CURLE_OK)
    , mRxStream()
{
    mRxStream.open(QBuffer::ReadWrite);

    mEasyHandle = curl_easy_init();

    bool success = true;

    success &= (CURLE_OK == curl_easy_setopt(mEasyHandle, CURLOPT_HEADERFUNCTION, &responseHeaderDispatcherCallback));
    success &= (CURLE_OK == curl_easy_setopt(mEasyHandle, CURLOPT_WRITEHEADER, this));

    success &= (CURLE_OK == curl_easy_setopt(mEasyHandle, CURLOPT_WRITEFUNCTION, &writeDispatcherCallback));
    success &= (CURLE_OK == curl_easy_setopt(mEasyHandle, CURLOPT_WRITEDATA, this));

    success &= (CURLE_OK == curl_easy_setopt(mEasyHandle, CURLOPT_DEBUGFUNCTION, &curlDebugDispatcherCallback));
    success &= (CURLE_OK == curl_easy_setopt(mEasyHandle, CURLOPT_DEBUGDATA, this));

    success &= (CURLE_OK == curl_easy_setopt(mEasyHandle, CURLOPT_TIMEOUT_MS, DEFAULT_OPERATION_TIMEOUT_MS));

    if (false == success) {
        WARN() << "[" << mId << "] Constructor: cannot init easy handle";
    }
    setVerboseLogging(true);
}

//--------------------------------------------------------------------------------------------------
ProtocolSlave::~ProtocolSlave()
//--------------------------------------------------------------------------------------------------
{
    INFO() << "[" << mId << "] destructor";
    mRxStream.close();

    curl_easy_cleanup(mEasyHandle);

    mEasyHandle = 0;
}

//--------------------------------------------------------------------------------------------------
int32_t ProtocolSlave::id() const
//--------------------------------------------------------------------------------------------------
{
    return mId;
}

//--------------------------------------------------------------------------------------------------
void ProtocolSlave::processExecutionResult(const CURLcode executionResult)
//--------------------------------------------------------------------------------------------------
{
    INFO() << "[" << mId << "] Execution result: " << executionResult << "[" << curlCodeToText.value(executionResult, "Unknown") << "]";
    mActive = false;
    mResult = executionResult;
}


void ProtocolSlave::setUrl(const QString &url)
{
    if (CURLE_OK != curl_easy_setopt(mEasyHandle, CURLOPT_URL, url.toStdString().c_str())) {
        WARN() << "[" << mId << "] Cannot set url";
    }

}

void ProtocolSlave::setTimeout(const uint32_t ms)
{
    if (CURLE_OK != curl_easy_setopt( mEasyHandle, CURLOPT_TIMEOUT_MS, ms)) {
        WARN() << "[" << mId << "] Cannot set timeout";
    }
}

void ProtocolSlave::setPort(const uint32_t port)
{
    if (CURLE_OK != curl_easy_setopt( mEasyHandle, CURLOPT_PORT, port)) {
        WARN() << "[" << mId << "] Cannot set port";
    }
}

void ProtocolSlave::setVerboseLogging(bool activate)
{
    if (CURLE_OK != curl_easy_setopt(mEasyHandle, CURLOPT_VERBOSE, activate ? 1 : 0)) {
        WARN() << "[" << mId << "] Cannot set verbose logging";
    }
}

bool ProtocolSlave::active() const
{
    return mActive;
}

void ProtocolSlave::setActive(bool active)
{
    mActive = active;
}

QByteArray ProtocolSlave::responseData() const
{
    return mRxStream.buffer();
}

QStringList ProtocolSlave::responseHeader() const
{
    return header;
}

CURL *ProtocolSlave::easyHandle() const
{
    return mEasyHandle;
}

CURLcode ProtocolSlave::result() const
{
    return mResult;
}

uint16_t ProtocolSlave::responseCode() const
{
    uint32_t code = 0;

    if (CURLE_OK != curl_easy_getinfo(mEasyHandle, CURLINFO_RESPONSE_CODE, &code)) {
        WARN() << "[" << mId << "] Response code = " << code;
    }

    if (0xFFFF < code) {
        WARN() << "[" << mId << "] Wrong response code: " << code;
        code = 400;
    }

    return static_cast<uint16_t>(code);
}

size_t ProtocolSlave::responseHeaderDispatcherCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    ProtocolSlave *slave = static_cast<ProtocolSlave *>(stream);

    if (slave) {
        return slave->responseHeaderCallback(ptr, size, nmemb);
    } else {
        return 0u;
    }
}

size_t ProtocolSlave::responseHeaderCallback(void *ptr, size_t size, size_t nmemb)
{
    size_t dataSize = size * nmemb;

    QString str(QString::fromLocal8Bit(reinterpret_cast<const char *>(ptr), static_cast<int>(dataSize)));
    str = str.trimmed(); // remove trailing "\n"
    header.append(str);

    return dataSize;
}

size_t ProtocolSlave::writeDispatcherCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
    ProtocolSlave *slave = static_cast<ProtocolSlave *>(stream);

    if (slave) {
       return slave->writeCallback(ptr, size, nmemb);
    } else {
       return 0u;
    }
}

size_t ProtocolSlave::writeCallback(void *ptr, size_t size, size_t nmemb)
{
    size_t bytes = size * nmemb;

    QByteArray data(static_cast<char *>(ptr), static_cast<int>(bytes));
    return static_cast<size_t>(mRxStream.write(data));

}

int ProtocolSlave::curlDebugDispatcherCallback(CURL *curl, curl_infotype infoType, char *text, size_t size, void *data)
{
    ProtocolSlave *slave = static_cast<ProtocolSlave *>(data);

    if (slave) {
        return slave->curlDebugCallback(curl, infoType, text, size);
    } else {
        return 0u;
    }
}

int32_t ProtocolSlave::curlDebugCallback(CURL */*curl*/, curl_infotype infoType, char *text, size_t inSize)
{
    if ((CURLINFO_TEXT       == infoType)
     || (CURLINFO_HEADER_IN  == infoType)
     || (CURLINFO_HEADER_OUT == infoType))
    {
        int size = static_cast<int>(inSize);

        QString out(QString::fromLocal8Bit(text, size).trimmed());

        // split the string to lines with max 150 symbols
        const int32_t maxPartLen = 150;

        QStringList lines = out.split("\r\n");

        for (int i = 0; i < lines.size(); i++) {
            int32_t pos = 0;

            while (lines[i].size() != pos) {
                int32_t len = lines[i].size() - pos;

                if (len > maxPartLen) {
                    len = maxPartLen;
                }

                INFO() << "libcurl [" << mId << "]: " << lines[i].mid(pos, len).trimmed();
                pos += len;
            }
        }
    }

    return 0u;
}
