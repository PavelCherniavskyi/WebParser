#pragma once

#include <QObject>
#include <QVector>
#include <QSharedPointer>

#include "../../Http/src/HttpTypes.h"
#include "src/private/Parser.h"

class Http;
class ResponseHandler;
class Provisioning;

class IDownloadManager: public QObject
{
    Q_OBJECT
public:
    IDownloadManager(QObject *obj = 0) : QObject(obj) {}
    virtual void execute() = 0;
    virtual void setResponseData(const ResponseDataParams value) = 0;
    virtual ~IDownloadManager() {}
signals:
    void doneExecution();
};

class DownloadManager : public IDownloadManager
{
    Q_OBJECT
public:
    struct ProvData {
        QList<QString> urls;
        uint timeout;
        QMap<Parser::ParseType, bool> parseTypes;
    };

    DownloadManager(QObject *obj = 0);
    void init(Provisioning *prov);
    void buildClients(QSharedPointer<Http> http);
    void execute() override;
    void setResponseData(const ResponseDataParams value) override;

private slots:
    void OnProvDataReceived(DownloadManager::ProvData provData);

private:
    bool                                        checkPreconditions();

    QSharedPointer<Http>                        m_http;
    QVector<ResponseDataParams>                 responseHandlers;
    ProvData                                    mProvData;
    Parser                                      parser;
};
