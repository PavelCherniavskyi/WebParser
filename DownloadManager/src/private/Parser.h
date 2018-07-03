#pragma once

#include <QObject>
#include <QMap>
#include <QVector>
#include <QRegularExpression>
#include "../../../Http/src/HttpTypes.h"

class IParser : public QObject
{
public:

};


class Parser : public IParser
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
    void parseLinks();

signals:
    void foundLink(QString url);

private:
    void execute(QString buffer, QRegularExpression reg, QStringList &out);
};
