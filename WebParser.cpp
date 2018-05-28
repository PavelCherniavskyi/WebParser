#include "WebParser.h"
#include "types.h"


WebParser::WebParser(int &argc, char **argv) : QCoreApplication(argc,argv)
{

}

void WebParser::init()
{
    registerMetatypes();
    provisioning = QSharedPointer<Provisioning>(new Provisioning(this));
    http = QSharedPointer<Http>(new Http(this));

    SmartLogger::init(provisioning.data());
    http.data()->init(provisioning.data());


    provisioning.data()->getProvisioning();


}
