#pragma once

#include <string>
#include <QMap>
#include <QVariant>
#include <vector>

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
    int32_t              id;
    std::vector<uint8_t> data;
    ResponseDataParams()
        : id(0)
        , data(std::vector<uint8_t>())
    {
    }

    ResponseDataParams(int32_t inId, const std::vector<uint8_t> &inData)
        : id(inId)
        , data(inData)
    {
    }
};

struct ProcessingFinishedParams
{
    int32_t   id;
    HttpError error;
    ProcessingFinishedParams()
        : id(0)
        , error(HttpError())
    {

    }
    ProcessingFinishedParams(int32_t inId, const HttpError &inError)
        : id(inId)
        , error(inError)
    {
    }
};
