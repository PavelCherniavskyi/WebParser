#pragma once

#include "HttpEnums.h"


struct ResponseDataParams
{
    int32_t             id;
    QByteArray          data;
    QStringList         header;
    HttpError           error;
    ResponseDataParams()
        : id(0)
        , data(QByteArray())
        , header()
        , error(HttpError())
    {
    }

    ResponseDataParams(int32_t inId, const QByteArray inData, const HttpError &inError, const QStringList inHeader)
        : id(inId)
        , data(inData)
        , header(inHeader)
        , error(inError)
    {
    }
};
