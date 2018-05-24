#pragma once

#include <QObject>
#include "Http.h"
#include "SmartLogger.h"

Q_DECLARE_METATYPE(Http::ProvData)
Q_DECLARE_METATYPE(SmartLogger::ProvData)
Q_DECLARE_METATYPE(SmartLogger::LOGWAY)

void registerMetatypes() {
    qRegisterMetaType<Http::ProvData>("Http::ProvData");
    qRegisterMetaType<SmartLogger::ProvData>("SmartLogger::ProvData");
    qRegisterMetaType<SmartLogger::LOGWAY>("SmartLogger::LOGWAY");
}
