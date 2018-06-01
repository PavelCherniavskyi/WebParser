#include "../Http.h"
#include "../Provisioning/src/Provisioning.h"
#include "../../../../WebParser/IRequestSender.h"

Http::Http(QObject *obj) : QObject(obj)
{

}

Http::~Http()
{
}

void Http::init(Provisioning *prov)
{
    mLibCurlConfig.init();
    connect(prov, &Provisioning::onHttpDataRecieved, this, &Http::OnProvDataReceived);
}

void Http::sendRequest(QSharedPointer<IRequestSender> sender, const QString &url, const QVector<uint8_t> &data)
{
    INFO() << "sendRequest:";
    INFO() << "url =" << url;
    INFO() << "data: " << data.size() << "bytes";

    QSharedPointer<ProtocolMaster> protocol = QSharedPointer<ProtocolMaster>::create(sender, mProtocolIdGenerator);

    connect(protocol.data(), &ProtocolMaster::addProtocolToProcessing,       mProcessorMaster.data(), &ProcessorMaster::addProtocolToProcessing     );
    connect(protocol.data(), &ProtocolMaster::removeProtocolFromProcessing,  mProcessorMaster.data(), &ProcessorMaster::removeProtocolFromProcessing);

    mProtocolMasters.push_back(protocol);

    sender->responseSendRequest(mProtocolIdGenerator++);

    protocol->sendRequest(url, QByteArray(reinterpret_cast<const char *>(data.data()), static_cast<int>(data.size())));
}

void Http::OnProvDataReceived(Http::ProvData provData)
{
    INFO() << "Urls received: " << provData.urls.size() << ". Timeout received: " << provData.timeout;
    mProvData = provData;
}
