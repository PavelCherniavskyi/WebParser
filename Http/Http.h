#pragma once

#include <QObject>

class Http : public QObject
{
    Q_OBJECT
public:
    Http() {}

public slots:
    void onProvisioningChanged(){}

private:
    bool isProvisioning {false};

};
