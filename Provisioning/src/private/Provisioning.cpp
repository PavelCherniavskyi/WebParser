#include "../Provisioning.h"

#include <QXmlQuery>
#include <QDebug>

bool Provisioning::getProvisioning()
{
    QString strListRes;
    QFile provData("../WebParser/Provisioning/provData.xml");
    if(provData.open(QIODevice::ReadOnly)) {
        INFO() << "Opened file susseccfully";

        QXmlQuery query;
        query.setFocus(&provData);
        query.setQuery("params/Http/none/text()");
        if (!query.isValid())
            WARN() << "Incorrect query!";

        if(!query.evaluateTo(&strListRes)) {
            WARN() << "Can't evaluate to string!";
        }

        provData.close();

        INFO() << strListRes.remove(strListRes.size() - 1, 1);

    } else {
        WARN() << "Can't open provData.xml";
    }


}
