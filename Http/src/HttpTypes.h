#pragma once

#include <string>
#include <QMap>
#include <QVariant>
#include <QVector>

#include "HttpEnums.h"


struct ResponseInfoParams
{
//    int32_t         id;
//    uint16_t        httpCode;
//    std::vector<dummtypes::DUMMTypes::HTTPHeader> headers;
//    std::vector<telematictypes::TelematicTypes::HTTPCookie> cookies;
//    ResponseInfoParams()
//    : id(0)
//    , httpCode(0u)
//    , headers(std::vector<dummtypes::DUMMTypes::HTTPHeader>())
//    , cookies(std::vector<telematictypes::TelematicTypes::HTTPCookie>())
//    {
//    }

//    /*! \brief Constructor with all ResponseInfo parameters */
//    ResponseInfoParams(int32_t inId, uint16_t inHttpCode, const std::vector<dummtypes::DUMMTypes::HTTPHeader> &inHeaders, const std::vector<telematictypes::TelematicTypes::HTTPCookie> &inCookies)
//    : id(inId)
//    , httpCode(inHttpCode)
//    , headers(inHeaders)
//    , cookies(inCookies)
//    {
//    }
};

struct ResponseDataParams
{
    int32_t             id;
    QByteArray          data;
    HttpError           error;
    ResponseDataParams()
        : id(0)
        , data(QByteArray())
        , error(HttpError())
    {
    }

    ResponseDataParams(int32_t inId, const QByteArray inData, const HttpError &inError)
        : id(inId)
        , data(inData)
        , error(inError)
    {
    }
};
