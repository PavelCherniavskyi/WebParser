#include <QCoreApplication>

#include "types.h"
#include "SmartLogger/src/SmartLogger.h"



int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SmartLogger::initLogger(SmartLogger::LOGPATH::LogToStdOut);

//    Provisioning prov;
//    prov.getProvisioning();

    return app.exec();
}
