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

    bool flag = false;
    for(size_t i = 0; i < extraLinks.size(); ++i) {
        for(size_t j = 0; j < value.header.size(); ++j) {
            if(value.header.at(j).contains(extraLinks.at(i))){
                extraLinks.removeAt(i);
                flag = true;
                break;
            }
        }
        if(flag){
            break;
        }
    }

    if(responseHandlers.size() >= mProvData.urls.size()) {
        INFO() << "Execution complete. Got " << responseHandlers.size() << " responses.";
        parser.doParse(responseHandlers, mProvData.parseTypes);
        emit doneExecution();
    }


}

void DownloadManager::gotExtraLink(QString url)
{
    m_http->sendRequest(this, url);
    QRegularExpression reg("[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}");
    QRegularExpressionMatch match = reg.match(url);
    if (match.hasMatch()) {
        extraLinks.append(match.captured(0));
    }
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
