#pragma once

#include <QObject>
#include "Http.h"
#include "SmartLogger.h"
#include "DownloadManager.h"

Q_DECLARE_METATYPE(DownloadManager::ProvData)
Q_DECLARE_METATYPE(SmartLogger::ProvData)
Q_DECLARE_METATYPE(SmartLogger::LOGWAY)

void registerMetatypes() {
    qRegisterMetaType<DownloadManager::ProvData>("DownloadManager::ProvData");
    qRegisterMetaType<SmartLogger::ProvData>("SmartLogger::ProvData");
    qRegisterMetaType<SmartLogger::LOGWAY>("SmartLogger::LOGWAY");
}
