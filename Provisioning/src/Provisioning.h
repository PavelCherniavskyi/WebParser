#pragma once

#include <QObject>
#include <QVector>

#include "../../Http/src/Http.h"
#include "../../SmartLogger/src/SmartLogger.h"

class Provisioning : public QObject
{
    Q_OBJECT
public:
    enum class ProvTypes
    {
        HTTP,
        SMART_LOGGER
    };

    Provisioning(QObject * obj = 0);
    bool getProvisioning();

signals:
    void onSmartLoggerDataRecieved(SmartLogger::ProvData provData);
    void onHttpDataRecieved(Http::ProvData provData);


private:



};
