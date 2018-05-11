#include <QCoreApplication>

#include "types.h"
#include "Provisioning.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Provisioning prov;
    prov.getProvisioning();

    return app.exec();
}
