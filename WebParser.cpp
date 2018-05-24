#include "WebParser.h"
#include "types.h"


WebParser::WebParser(int &argc, char **argv) : QCoreApplication(argc,argv)
{

}

void WebParser::init()
{
    registerMetatypes();
    provisioning = QSharedPointer<Provisioning>(new Provisioning(this));
    SmartLogger::init(provisioning.data());
    Http::init(provisioning.data());


    provisioning.data()->getProvisioning();


}
