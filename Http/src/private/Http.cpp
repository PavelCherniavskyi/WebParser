#include "../Http.h"
#include "../Provisioning/src/Provisioning.h"

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
    (void)prov; //for future
    mLibCurlConfig.init();
}

void Http::sendRequest(DownloadManager *sender, const QString &url)
{
    if(sender == nullptr) {
        ERROR() << "DownloadManager is Empty";
    }

    INFO() << "url =" << url;

    QSharedPointer<ProtocolMaster> protocol = QSharedPointer<ProtocolMaster>::create(sender, mProtocolIdGenerator++, this);

    connect(protocol.data(), &ProtocolMaster::addProtocolToProcessing,       mProcessorMaster.data(), &ProcessorMaster::addProtocolToProcessing     );
    connect(protocol.data(), &ProtocolMaster::removeProtocolFromProcessing,  mProcessorMaster.data(), &ProcessorMaster::removeProtocolFromProcessing);

    mProtocolMasters.push_back(protocol);

    protocol->sendRequest(url);
}

void Http::protocolProcessingFinished(qint32 id)
{
    INFO() << "Id:  " << id;
    auto iter = std::find_if(mProtocolMasters.begin(), mProtocolMasters.end()
                             , [id](QSharedPointer<ProtocolMaster> proto)
                               {return proto->id() == id;});

    if (mProtocolMasters.end() != iter) {
        mProtocolMasters.erase(iter);
    }
}
