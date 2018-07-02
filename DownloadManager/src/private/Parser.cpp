#include "Parser.h"


Parser::Parser()
{
}

void Parser::doParse(QVector<ResponseDataParams> params, QMap<ParseType, bool> parseTypes)
{
    foreach (const ResponseDataParams par, params) {
        QString buffer(par.data.toStdString());

    }
}
