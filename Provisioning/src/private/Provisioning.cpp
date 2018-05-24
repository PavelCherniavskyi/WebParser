#include <QXmlQuery>
#include <QtXml>
#include <QDebug>

#include "../Provisioning.h"

Provisioning::Provisioning(QObject *obj) : QObject(obj)
{

}

bool Provisioning::getProvisioning()
{

    Http::ProvData httpProvData;
    QFile provData("../WebParser/ProvData.xml");
    if(provData.open(QIODevice::ReadOnly)) {
        qDebug() << "Opened file susseccfully";

        QXmlQuery query;
        query.setFocus(&provData);

        query.setQuery("provdata/http/url/text()");
        query.evaluateTo(&httpProvData.url);

        query.setQuery("provdata/http/method/text()");
        query.evaluateTo(&httpProvData.method);

        query.setQuery("provdata/http/timeout/text()");
        QString temp;
        query.evaluateTo(&temp);
        httpProvData.timeout = static_cast<qint16>(temp.toInt());

        provData.close();

        emit onHttpDataRecieved(httpProvData);

    } else {
        qDebug() << "Can't open provData.xml";
    }

    return true;
}
