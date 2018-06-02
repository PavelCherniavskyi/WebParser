#include "WebParser.h"
#include "types.h"


WebParser::WebParser(int &argc, char **argv) : QCoreApplication(argc,argv)
{
    INFO();
}

WebParser::~WebParser()
{
    INFO();
}

void WebParser::init()
{
    registerMetatypes();
    provisioning = QSharedPointer<Provisioning>(new Provisioning(this));
    http = QSharedPointer<Http>(new Http(this));

    SmartLogger::init(provisioning.data());
    http.data()->init(provisioning.data());


    provisioning.data()->getProvisioning();

    http->sendRequest(QSharedPointer<IRequestSender>(this), QString("http://dumskaya.net/"));
}

void WebParser::responseSendRequest(int32_t id)
{
    INFO() << "Webparser sends request: " << id;
}

void WebParser::informationProcessingFinishedValue(const ProcessingFinishedParams &value)
{
    INFO() << "Got finished value: " << value.id << " " << HttpErrorToText[value.error];
}
