#pragma once

#include <QObject>
#include "private/LibCurlConfig.h"

class Provisioning;

class Http : public QObject
{
    Q_OBJECT
public:
    struct ProvData {
        QList<QString> urls;
        uint timeout;
    };

    Http(QObject *obj) : QObject(obj) {}
    ~Http();
    void init(Provisioning* prov);

private slots:
    void OnProvDataReceived(Http::ProvData provData);

private:
    LibCurlConfig mLibCurlConfig;
    ProvData mProvData;
};
