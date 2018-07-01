#include "../DownloadManager.h"
#include "../../../Http/src/Http.h"
#include "../../../SmartLogger/src/SmartLogger.h"
#include "../../../Provisioning/src/Provisioning.h"

DownloadManager::DownloadManager(QObject *obj) : IDownloadManager(obj)
{

}

void DownloadManager::init(Provisioning *prov)
{
    connect(prov, &Provisioning::onDownloadMngrDataRecieved, this, &DownloadManager::OnProvDataReceived);
}

void DownloadManager::buildClients(QSharedPointer<Http> http)
{
    m_http = http;
}

void DownloadManager::execute()
{
    if(!checkPreconditions()) {
        return;
    }

    foreach (const QString url, mProvData.urls) {
        m_http->sendRequest(this, url);
    }

}

void DownloadManager::setResponseData(const ResponseDataParams value)
{
    INFO() << "Response data [" << value.id << "]: Bytes: " << value.data.size() << ". Error: " << HttpErrorToText[value.error];
    responseHandlers.push_back(value);

    if(responseHandlers.size() == mProvData.urls.size()) {
        INFO() << "Execution complete. Got " << responseHandlers.size() << " responses.";
        parser.doParse(responseHandlers, mProvData.parseTypes);
        emit doneExecution();
    }
}

void DownloadManager::OnProvDataReceived(DownloadManager::ProvData provData)
{
    INFO() << "Urls received: " << provData.urls.size() << ". Timeout received: " << provData.timeout;
    mProvData = provData;
}

bool DownloadManager::checkPreconditions()
{
    bool success = true;
    if(m_http == nullptr) {
        ERROR() << "Http is empty. Need to build clients first!";
        success = false;
    }
    if(mProvData.urls.isEmpty()) {
        WARN() << "No urls in Provdata";
        success = false;
    }

    return success;
}
