#include <QCoreApplication>

#include "types.h"
#include "Provisioning.h"
#include "SmartLogger.h"



int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "Some info";

    SmartLogger logger;
    logger.initLogger();

    Provisioning prov;
    prov.getProvisioning();

    return app.exec();
}
