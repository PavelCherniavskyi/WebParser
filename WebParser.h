#pragma once

#include <QObject>
#include <QCoreApplication>
#include <QSharedPointer>
#include <QList>

#include "SmartLogger.h"
#include "Http.h"
#include "Provisioning.h"
#include "DownloadManager.h"

class WebParser : public QCoreApplication
{
public:
    WebParser(int &argc, char **argv);
    ~WebParser();
    void init();
    void run();

private:
    QSharedPointer<Provisioning> provisioning;
    QSharedPointer<Http> http;
    QSharedPointer<DownloadManager> downloadManager;
};
