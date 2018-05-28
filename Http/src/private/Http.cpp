#include "../Http.h"
#include "../Provisioning/src/Provisioning.h"

Http::~Http()
{
}

void Http::init(Provisioning *prov)
{
    mLibCurlConfig.init();
    connect(prov, &Provisioning::onHttpDataRecieved, this, &Http::OnProvDataReceived);
}

void Http::OnProvDataReceived(Http::ProvData provData)
{
    INFO() << "Provisionig received: " << provData.method << provData.url << provData.timeout;
}
