#pragma once

#include <QObject>
#include "Http.h"
#include "SmartLogger.h"

Q_DECLARE_METATYPE(Http::ProvData)
Q_DECLARE_METATYPE(SmartLogger::ProvData)
Q_DECLARE_METATYPE(SmartLogger::LOGWAY)
//Q_DECLARE_METATYPE(ProtocolMaster)
//Q_DECLARE_METATYPE(ProcessorMaster)

void registerMetatypes() {
    qRegisterMetaType<Http::ProvData>("Http::ProvData");
    qRegisterMetaType<SmartLogger::ProvData>("SmartLogger::ProvData");
    qRegisterMetaType<SmartLogger::LOGWAY>("SmartLogger::LOGWAY");
    //qRegisterMetaType<ProtocolMaster>("ProtocolMaster");
    //qRegisterMetaType<ProcessorMaster>("ProcessorMaster");
}
