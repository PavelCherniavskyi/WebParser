#pragma once

#include <QObject>
#include <QVector>

#include "../../DownloadManager/src/DownloadManager.h"
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
    ~Provisioning();
    bool getProvisioning(QString path = "");

signals:
    void onSmartLoggerDataRecieved(SmartLogger::ProvData provData);
    void onDownloadMngrDataRecieved(DownloadManager::ProvData provData);


private:



};
