#include "Parser.h"
#include "../../../SmartLogger/src/SmartLogger.h"

Parser::Parser()
{
}

void Parser::doParse(ResponseDataParams param, QMap<ParseType, bool> parseTypes)
{
    bool isPhone = parseTypes.value(ParseType::Phones);
    bool isEmail = parseTypes.value(ParseType::Emails);
    QRegularExpression reg;
    if(isPhone) {
        reg.setPattern("\\b(((\\+\\s?3\\s?8)(-?|\\s)0(-?|\\s)\\d(-?|\\s)\\d(-?|\\s))|0(-?|\\s)\\d(-?|\\s)\\d(-?|\\s))\\d(-?|\\s)\\d(-?|\\s)\\d(-?|\\s)\\d(-?|\\s)\\d(-?|\\s)\\d(-?|\\s)\\d(-?|\\s)\\b");
        execute(QString(param.data.toStdString().c_str()), reg, phones);
    }
    if(isEmail) {
        reg.setPattern("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,6}\\b");
        execute(QString(param.data.toStdString().c_str()), reg, emails);
    }

    parseLinks(param);
}

void Parser::parseLinks(ResponseDataParams param)
{
    QRegularExpressionMatch matchUrl;
    param.url.contains(QRegularExpression("https?://"), &matchUrl);
    QString urlPrefix = matchUrl.captured(0);
    param.url.contains(QRegularExpression("[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}"), &matchUrl);
    QString urlClear = matchUrl.captured(0) + '/';

    QString pattern = "href=/([A-Za-z0-9-]+/){1,}\\s";
    QRegularExpression reg(pattern);
    QRegularExpressionMatchIterator iter = reg.globalMatch(QString(param.data.toStdString().c_str()));
    while (iter.hasNext()) {
        QRegularExpressionMatch match = iter.next();
        if (match.hasMatch()) {
            match.captured(0).contains(QRegularExpression("([A-Za-z0-9-]+/){1,}\\s"), &matchUrl);
            INFO() << "Found link: " << urlPrefix + urlClear + matchUrl.captured(0);
            emit foundLink(urlPrefix + urlClear + matchUrl.captured(0));
        }
    }
}

void Parser::execute(QString buffer, QRegularExpression reg, QStringList &out)
{
    QRegularExpressionMatchIterator iter = reg.globalMatch(buffer);
    while (iter.hasNext()) {
        QRegularExpressionMatch match = iter.next();
        if (match.hasMatch()) {
                 out.append(match.captured(0));
            }
    }
    out.removeDuplicates();
}
