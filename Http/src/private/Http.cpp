#include "../Http.h"
#include "../Provisioning/src/Provisioning.h"
#include "../../../../WebParser/IRequestSender.h"

Http::Http(QObject *obj) : QObject(obj)
  , mProcessorMaster()
  , mProcessorExecutor()
  , mProtocolIdGenerator(0)
{
    mProcessorMaster = QSharedPointer<ProcessorMaster>::create(0);

    connect(mProcessorMaster.data(), &ProcessorMaster::addProcessorToExecution,      &mProcessorExecutor, &ProcessorExecutor::addProcessorToExecution     );
    connect(mProcessorMaster.data(), &ProcessorMaster::removeProcessorFromExecution, &mProcessorExecutor, &ProcessorExecutor::removeProcessorFromExecution);
    connect(mProcessorMaster.data(), &ProcessorMaster::protocolProcessingFinished,   this,                &Http::protocolProcessingFinished, Qt::QueuedConnection);
}

Http::~Http()
{
}

void Http::init(Provisioning *prov)
{
    mLibCurlConfig.init();
    connect(prov, &Provisioning::onHttpDataRecieved, this, &Http::OnProvDataReceived);
}

void Http::sendRequest(QSharedPointer<IRequestSender> sender, const QString &url)
{
    INFO() << "sendRequest:";
    INFO() << "url =" << url;

    QSharedPointer<ProtocolMaster> protocol = QSharedPointer<ProtocolMaster>::create(sender, mProtocolIdGenerator);

    connect(protocol.data(), &ProtocolMaster::addProtocolToProcessing,       mProcessorMaster.data(), &ProcessorMaster::addProtocolToProcessing     );
    connect(protocol.data(), &ProtocolMaster::removeProtocolFromProcessing,  mProcessorMaster.data(), &ProcessorMaster::removeProtocolFromProcessing);

    mProtocolMasters.push_back(protocol);

    sender->responseSendRequest(mProtocolIdGenerator++);

    protocol->sendRequest(url);
}

void Http::OnProvDataReceived(Http::ProvData provData)
{
    INFO() << "Urls received: " << provData.urls.size() << ". Timeout received: " << provData.timeout;
    mProvData = provData;
}

void Http::protocolProcessingFinished(qint32 id)
{
    INFO();
    auto iter = std::find_if(mProtocolMasters.begin(), mProtocolMasters.end()
                             , [id](QSharedPointer<ProtocolMaster> proto)
                               {return proto->id() == id;});

    if (mProtocolMasters.end() != iter) {
        mProtocolMasters.erase(iter);
    }
}
