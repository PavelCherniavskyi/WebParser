#pragma once

#include "HttpEnums.h"


struct ResponseDataParams
{
    int32_t             id;
    QByteArray          data;
    QStringList         header;
    HttpError           error;
    QString             url;
    ResponseDataParams()
        : id(0)
        , data(QByteArray())
        , header()
        , error(HttpError())
        , url()
    {
    }

    ResponseDataParams(int32_t inId, const QByteArray inData, const HttpError &inError, const QStringList inHeader, const QString inUrl)
        : id(inId)
        , data(inData)
        , header(inHeader)
        , error(inError)
        , url(inUrl)
    {
    }
};
