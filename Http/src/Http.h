#pragma once

#include <QObject>
#include "LibCurlConfig.h"

class Provisioning;

class Http : public QObject
{
    Q_OBJECT
public:
    struct ProvData {
        QString url;
        QString method;
        quint16 timeout;
    };

    Http(QObject *obj) : QObject(obj) {}
    ~Http();
    void init(Provisioning* prov);

private slots:
    void OnProvDataReceived(Http::ProvData provData);

private:
    LibCurlConfig mLibCurlConfig;

};
