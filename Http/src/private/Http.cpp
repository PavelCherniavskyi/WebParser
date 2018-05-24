#include "../Http.h"
#include "../Provisioning/src/Provisioning.h"

#include <QDebug> //temp

void Http::init(Provisioning *prov)
{
    connect(prov, &Provisioning::onHttpDataRecieved, OnProvDataReceived);
}

void Http::OnProvDataReceived(Http::ProvData provData)
{
    qDebug() << provData.method << provData.url << provData.timeout;
}
