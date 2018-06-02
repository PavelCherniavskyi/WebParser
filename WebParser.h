#pragma once

#include <QObject>
#include <QCoreApplication>
#include <QSharedPointer>

#include "SmartLogger.h"
#include "Http.h"
#include "Provisioning.h"

class WebParser : public QCoreApplication, public IRequestSender
{
public:
    WebParser(int &argc, char **argv);
    ~WebParser();
    void init();
    void responseSendRequest(int32_t id) override;
    void informationProcessingFinishedValue(const ProcessingFinishedParams& value) override;

private:
    QSharedPointer<Provisioning> provisioning;
    QSharedPointer<Http> http;
};
