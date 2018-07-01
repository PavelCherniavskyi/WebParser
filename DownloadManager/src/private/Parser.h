#pragma once

#include <QObject>
#include <QMap>
#include <QVector>
#include "../../../Http/src/HttpTypes.h"

class IParser : QObject
{
public:

};


class Parser : IParser
{
    Q_OBJECT
public:
    enum ParseType{
        Phones,
        Emails
    };
    Q_ENUM(ParseType)
    Parser();
    void doParse(QVector<ResponseDataParams> params, QMap<ParseType, bool> parseTypes);
};
