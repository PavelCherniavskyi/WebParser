#pragma once

#include <QObject>
#include <QCoreApplication>
#include <QSharedPointer>

#include "SmartLogger.h"
#include "Http.h"
#include "Provisioning.h"

class WebParser : public QCoreApplication
{
public:
    WebParser(int &argc, char **argv);
    void init();

private:
    QSharedPointer<Provisioning> provisioning;
};
