#include "Parser.h"
#include "../../../SmartLogger/src/SmartLogger.h"

Parser::Parser()
{
}

void Parser::doParse(QVector<ResponseDataParams> params, QMap<ParseType, bool> parseTypes)
{
    bool isPhone = parseTypes.value(ParseType::Phones);
    bool isEmail = parseTypes.value(ParseType::Emails);
    QRegularExpression reg;
    if(isPhone) {
        INFO() << "Parsing phones...";
        QStringList phones;
        reg.setPattern("\\b(((\\+\\s?3\\s?8)(-?|\\s)0(-?|\\s)\\d(-?|\\s)\\d(-?|\\s))|0(-?|\\s)\\d(-?|\\s)\\d(-?|\\s))\\d(-?|\\s)\\d(-?|\\s)\\d(-?|\\s)\\d(-?|\\s)\\d(-?|\\s)\\d(-?|\\s)\\d(-?|\\s)\\b");
        foreach (const ResponseDataParams par, params) {
            execute(QString(par.data.toStdString().c_str()), reg, phones);
            foreach (QString str, phones) {
                INFO() << str;
            }
            phones.clear();

        }
    }
    if(isEmail) {
        INFO() << "Parsing emails...";
        QStringList emails;
        reg.setPattern("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,6}\\b");
        foreach (const ResponseDataParams par, params) {
            execute(QString(par.data.toStdString().c_str()), reg, emails);
            foreach (QString str, emails) {
                INFO() << str;
            }
            emails.clear();
        }
    }
}

void Parser::parseLinks()
{

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
