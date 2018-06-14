#include "HttpTest.h"
#include "ExpressServer.h"
#include "../src/private/ProtocolSlave.h"
#include "../src/private/ProtocolMaster.h"
#include "../../../SmartLogger/src/SmartLogger.h"

void DownloadManagerMock::setResponseData(const ResponseDataParams value)
{
    responseHandlers.push_back(value);
}

void ProcessorMasterMock::jobExecuted()
{

}

void ProcessorMasterMock::addProtocolToProcessing(ProtocolMaster *protocol)
{
    if(protocol){
        mProtocolMasters.push_back(protocol);
    }

}

void ProcessorMasterMock::removeProtocolFromProcessing(ProtocolMaster *protocol)
{
    if (protocol) {
        auto iter = std::find(mProtocolMasters.begin(), mProtocolMasters.end(), protocol);
        mProtocolMasters.erase(iter);
    }
}

void ProcessorExecutorMock::addProcessorToExecution(ProcessorMaster *processor)
{
    //processor->slave()->execute();
}

void ProcessorExecutorMock::removeProcessorFromExecution(ProcessorMaster *processor)
{

}

void ProcessorExecutorMock::handleJobExecuted()
{

}

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
    QString arg = QDir::currentPath();
    arg += "/../../WebParser/Utils/NodeJsExpress/app.js";
    QStringList arguments = {arg};
    ExpressServer expressServer(arguments, this);
    QVERIFY(expressServer.startServer());

    DownloadManagerMock *manager = new DownloadManagerMock(this);
    ProtocolMaster *protocol = new ProtocolMaster(manager, 88, this);
    ProcessorMasterMock *processorMaster = new ProcessorMasterMock(this);
    connect(protocol,           &ProtocolMaster::addProtocolToProcessing,           processorMaster,    &ProcessorMasterMock::addProtocolToProcessing       );
    connect(protocol,           &ProtocolMaster::removeProtocolFromProcessing,      processorMaster,    &ProcessorMasterMock::removeProtocolFromProcessing  );
    protocol->sendRequest("http://localhost/protocolmaster", 3000);

    QCOMPARE(processorMaster->getProtocolMasters().size(), 1);

    CURLcode res;
    res = curl_easy_perform(protocol->slave()->easyHandle());

    QCOMPARE(res, CURLE_OK);

    protocol->jobExecuted();

    QVERIFY(processorMaster->getProtocolMasters().empty());

    QCOMPARE(manager->getResponseData().back().id, 88);
    QCOMPARE(manager->getResponseData().back().data, "Some data for ProtocolMaster");
    QCOMPARE(manager->getResponseData().back().error, 0);
    QCOMPARE(manager->getResponseData().back().header.size(), 8);
}

void HttpTest::cleanupTestCase()
{
}
