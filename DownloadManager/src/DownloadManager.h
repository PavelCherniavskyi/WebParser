#pragma once

#include <QObject>
#include <QVector>
#include <QSharedPointer>

#include "../../Http/src/HttpTypes.h"

class Http;
class ResponseHandler;
class Provisioning;

class DownloadManager : public QObject
{
    Q_OBJECT
public:
    struct ProvData {
        QList<QString> urls;
        uint timeout;
    };

    DownloadManager(QObject *obj = 0);
    void init(Provisioning *prov);
    void buildClients(Http *http);
    void execute();
    void setResponseData(const ResponseDataParams value);

signals:
    void doneExecution();

private slots:

    void OnProvDataReceived(DownloadManager::ProvData provData);

private:
    bool                                        checkPreconditions();

    Http                                        *m_http;
    QVector<ResponseDataParams>                 responseHandlers;
    ProvData                                    mProvData;
};
