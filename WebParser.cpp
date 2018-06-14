#include "WebParser.h"
#include "types.h"


WebParser::WebParser(int &argc, char **argv) : QCoreApplication(argc,argv)
{
    INFO();
}

WebParser::~WebParser()
{
    INFO();
}

void WebParser::init()
{
    registerMetatypes();
    provisioning = QSharedPointer<Provisioning>(new Provisioning(this));
    http = QSharedPointer<Http>(new Http(this));
    downloadManager = QSharedPointer<DownloadManager>(new DownloadManager(this));
    downloadManager->buildClients(http);

    connect(downloadManager.data(), &DownloadManager::doneExecution, this, &WebParser::quit);

    SmartLogger::init(provisioning.data());
    http->init(provisioning.data());
    downloadManager->init(provisioning.data());

    provisioning->getProvisioning();
}

void WebParser::run()
{
    downloadManager->execute();
}
