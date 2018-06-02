#pragma once
/*! \brief Enumerates the possible HTTP errors */
enum class HttpError : char {
    NO_ERROR,
    ID_INVALID,
    ERROR_GENERIC,
    TRANSFER_OPERATION_TIMEOUT,
    CONNECTION_CLOSE,
    ABORTED,
    ERROR_TLS
};
/*! \brief Enumerates the available HTTP options */
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
/*! \brief Enumerates the available response handler types */
enum class ResponseHandler : char {
    AGGREGATE,
    STREAM
};
