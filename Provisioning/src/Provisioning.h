#pragma once

#include "SmartLogger.h"

#include <QObject>
#include <QVector>
#include <QtXml>

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

private:



};
