#include "../Provisioning.h"

#include <QXmlQuery>
#include <QDebug>

Provisioning::Provisioning(QObject *obj) : QObject(obj)
{

}

bool Provisioning::getProvisioning()
{
    QString strListRes;
    QFile provData("../WebParser/ProvData.xml");
    if(provData.open(QIODevice::ReadOnly)) {
        qDebug() << "Opened file susseccfully";

        QXmlQuery query;
        query.setFocus(&provData);




        query.setQuery("provdata/http/url/text()");
        if (!query.isValid())
            qDebug() << "Incorrect query!";

        if(!query.evaluateTo(&strListRes)) {
            qDebug() << "Can't evaluate to string!";
        }

        query.setQuery("provdata/http/method/text()");
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
