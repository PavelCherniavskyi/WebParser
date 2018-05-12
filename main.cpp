#include <QCoreApplication>

#include "types.h"
#include "Provisioning.h"
#include "SmartLogger.h"



int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SmartLogger::initLogger();
    INFO() << "test info";

    //Provisioning prov;
    //prov.getProvisioning();

    return app.exec();
}
