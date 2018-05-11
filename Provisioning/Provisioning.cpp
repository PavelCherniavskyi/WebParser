#include "Provisioning.h"

#include <QXmlQuery>
#include <QDebug>

bool Provisioning::getProvisioning()
{
    QString strListRes;
    QFile provData("../WebParser/Provisioning/provData.xml");
    if(provData.open(QIODevice::ReadOnly)) {
        qDebug() << "Opened file susseccfully";

        QXmlQuery query;
        query.setFocus(&provData);
        query.setQuery("params/Http/none/text()");
        if (!query.isValid())
            qDebug() << "Incorrect query!";

        if(!query.evaluateTo(&strListRes)) {
            qDebug() << "Can't evaluate to string!";
        }

        provData.close();

        qDebug() << strListRes.remove(strListRes.size() - 1, 1);

    } else {
        qDebug() << "Can't open provData.xml";
    }


}
