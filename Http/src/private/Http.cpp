#include "../Http.h"
#include "../Provisioning/src/Provisioning.h"

void Http::init(Provisioning *prov)
{
    connect(prov, &Provisioning::onHttpDataRecieved, OnProvDataReceived);
}

void Http::OnProvDataReceived(Http::ProvData provData)
{
    INFO() << provData.method << provData.url << provData.timeout;
}
