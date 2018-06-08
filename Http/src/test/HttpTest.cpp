#include "HttpTest.h"
#include "ExpressServer.h"
#include "../src/private/ProtocolSlave.h"
#include "../../../SmartLogger/src/SmartLogger.h"

void HttpTest::initTestCase()
{
}

void HttpTest::protocolSlaveTest()
{
    QString arg = QDir::currentPath();
    arg += "/../../WebParser/Utils/NodeJsExpress/app.js";
    QStringList arguments = {arg};
    ExpressServer expressServer(arguments, this);
    QVERIFY(expressServer.startServer());

    ProtocolSlave slave(99);
    slave.setPort(3000);
    slave.setUrl("http://localhost/protocolslave");

    CURLcode res;
    res = curl_easy_perform(slave.easyHandle());

    QCOMPARE(res, CURLE_OK);

    QByteArray data = slave.responseData();
    QStringList headers = slave.responseHeader();
    uint16_t code = slave.responseCode();

    QCOMPARE(QString(data), "Some data for ProtocolSlave");
    QCOMPARE(headers.size(), 8);
    QCOMPARE(code, 200);
}

void HttpTest::protocolMasterTest()
{

}

void HttpTest::cleanupTestCase()
{
}
