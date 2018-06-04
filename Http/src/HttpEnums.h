#pragma once

#include <QObject>
#include <QMap>

enum HttpError {
    NO_ERROR,
    ID_INVALID,
    ERROR_GENERIC,
    TRANSFER_OPERATION_TIMEOUT,
    CONNECTION_CLOSE,
    ABORTED,
    ERROR_TLS
};

const QMap<HttpError, QString> HttpErrorToText = {
    {NO_ERROR,                      "NO_ERROR"},
    {ID_INVALID,                    "ID_INVALID"},
    {ERROR_GENERIC,                 "ERROR_GENERIC"},
    {TRANSFER_OPERATION_TIMEOUT,    "TRANSFER_OPERATION_TIMEOUT"},
    {CONNECTION_CLOSE,              "CONNECTION_CLOSE"},
    {ABORTED,                       "ABORTED"},
    {ERROR_TLS,                     "ERROR_TLS"}
};

enum class HttpOption : char {
    PROXY,
    TIMEOUT,
    COOKIES,
    HEADERS,
    CERTIFICATES,
    CLIENT_CERTIFICATE,
    RESPONSE_HANDLER,
    LOCAL_PORT_RANGE,
    SSL_VERIFYSTATUS
};
