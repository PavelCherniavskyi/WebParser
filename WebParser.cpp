#include "WebParser.h"


WebParser::WebParser(int &argc, char **argv) : QCoreApplication(argc,argv)
{

}

void WebParser::init()
{
    provisioning = QSharedPointer<Provisioning>(new Provisioning(this));
    SmartLogger::init(provisioning.data());


    provisioning.data()->getProvisioning();


}
