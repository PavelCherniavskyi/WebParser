#include "../DownloadManager.h"
#include "../../../Http/src/Http.h"
#include "../../../SmartLogger/src/SmartLogger.h"
#include "../../../Provisioning/src/Provisioning.h"
#include <QRegularExpression>

DownloadManager::DownloadManager(QObject *obj) : IDownloadManager(obj)
{

}

void DownloadManager::init(Provisioning *prov)
{
    connect(prov, &Provisioning::onDownloadMngrDataRecieved, this, &DownloadManager::OnProvDataReceived);
    connect(&parser, &Parser::foundLink, this, &DownloadManager::gotExtraLink);
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
    parser.doParse(value, mProvData.parseTypes);


    if(responseHandlers.size() == extraLinks.size() + mProvData.urls.size()) {
        INFO() << "Execution complete. Got " << responseHandlers.size() << " responses.";
        INFO() << "Urls:";
        foreach (auto url, responseHandlers) {
            INFO() << url.url;
        }
        INFO() << "Phones:";
        foreach (auto ph, parser.getPhones()) {
            INFO() << ph;
        }
        INFO() << "Emails:";
        foreach (auto em, parser.getEmails()) {
            INFO() << em;
        }
        emit doneExecution();
    }
}

void DownloadManager::gotExtraLink(QString url)
{
    foreach (auto hnld, extraLinks) {
        if(hnld.contains(url)){
            INFO() << "Double url found: " << url << " in " << hnld;
            return;
        }
    }

    extraLinks.append(url);
    m_http->sendRequest(this, url);
}

void DownloadManager::OnProvDataReceived(DownloadManager::ProvData provData)
{
    INFO() << "Urls received: " << provData.urls.size() << ". Timeout received: " << provData.timeout;
    INFO() << "Phones:" << provData.parseTypes.value(Parser::Phones) << " Emails:" << provData.parseTypes.value(Parser::Emails);
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
