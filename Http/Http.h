#pragma once

#include <QObject>

class Http : public QObject
{
    Q_OBJECT
public:
    struct ProvData {
        QString url;
        QString method;
        qint16 timeout;
    };

    Http() {}

public slots:
    void onProvisioningChanged(){}

private:
    bool isProvisioning {false};

};
