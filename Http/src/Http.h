#pragma once

#include <QObject>

class Provisioning;

class Http : public QObject
{
    Q_OBJECT
public:
    struct ProvData {
        QString url;
        QString method;
        qint16 timeout;
    };

    Http(QObject *obj) : QObject(obj) {}
    static void init(Provisioning* prov);

private slots:
    static void OnProvDataReceived(Http::ProvData provData);

private:


};
