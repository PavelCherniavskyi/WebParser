#include <QSignalSpy>
#include "HttpTest.h"
#include "JobExecutor.h"
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

void ProcessorMasterMock::addProtocolToProcessing(IProtocolMaster *protocol)
{
    if(protocol){
        mProtocolMasters.push_back(protocol);
    }

}

void ProcessorMasterMock::removeProtocolFromProcessing(IProtocolMaster *protocol)
{
    if (protocol) {
        auto iter = std::find(mProtocolMasters.begin(), mProtocolMasters.end(), protocol);
        mProtocolMasters.erase(iter);
    }
}

void ProcessorExecutorMock::addProcessorToExecution(ProcessorMaster *processor)
{
    if(processor) {
        mProcessorMasters.append(processor);
    }

}

void ProcessorExecutorMock::removeProcessorFromExecution(ProcessorMaster *processor)
{
    if (processor) {
        auto iter = std::find(mProcessorMasters.begin(), mProcessorMasters.end(), processor);
        mProcessorMasters.erase(iter);
    }
}

void ProcessorExecutorMock::handleJobExecuted()
{
}


bool ProtocolMasterMock::sendRequest(const QString &url, int port)
{
}

void ProtocolMasterMock::jobExecuted()
{
}

void HttpTest::initTestCase()
{
    QString arg = QDir::currentPath();
    arg += "/../../WebParser/Utils/NodeJsExpress/app.js";
    QStringList arguments = {arg};
    expressServer.setArguments(arguments);

    QVERIFY(expressServer.startServer());
}

void HttpTest::protocolSlaveTest()
{
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
    DownloadManagerMock *manager = new DownloadManagerMock(this);
    ProtocolMaster *protocol = new ProtocolMaster(manager, 88, this);
    ProcessorMasterMock *processorMaster = new ProcessorMasterMock(this);
    connect(protocol, &ProtocolMaster::addProtocolToProcessing,     processorMaster, &ProcessorMasterMock::addProtocolToProcessing     );
    connect(protocol, &ProtocolMaster::removeProtocolFromProcessing,processorMaster, &ProcessorMasterMock::removeProtocolFromProcessing);
    protocol->sendRequest("http://localhost/protocolmaster", 3000);

    QCOMPARE(processorMaster->getProtocolMasters().size(), 1);

    CURLcode res;
    res = curl_easy_perform(protocol->slave()->easyHandle());

    QCOMPARE(res, CURLE_OK);

    protocol->jobExecuted();

    QVERIFY(processorMaster->getProtocolMasters().empty());

    ResponseDataParams response = manager->getResponseData().back();
    QCOMPARE(response.id, 88);
    QCOMPARE(response.data, "Some data for ProtocolMaster");
    QCOMPARE(response.error, 0);
    QCOMPARE(response.header.size(), 8);
}

void HttpTest::processorSlaveTest()
{
    QScopedPointer<ProcessorSlave> procSlave(new ProcessorSlave(77));
    QScopedPointer<ProtocolSlave> protocolSlave(new ProtocolSlave(66));
    protocolSlave->setPort(3000);
    protocolSlave->setUrl("http://localhost/processorslave");

    QVERIFY(procSlave->add(protocolSlave.data()));

    fd_set fdRead, fdWrite, fdError;
    struct timeval timeVal;
    int maxFd = -1;
    int64_t timeout;

    timeout = procSlave->timeout();
    procSlave->execute();

    do
    {
        FD_ZERO(&fdRead);
        FD_ZERO(&fdWrite);
        FD_ZERO(&fdError);

        procSlave->descriptors(&fdRead, &fdWrite, &fdError, &maxFd);
        INFO() << maxFd;
        if (maxFd == -1) {
            QThread::msleep(timeout);
            INFO() << "Sleeping for: " << timeout;
        } else {
            timeVal.tv_sec = timeout / 1000;
            timeVal.tv_usec = (timeout % 1000 ) * 1000;
            select(maxFd + 1, &fdRead, &fdWrite, &fdError, &timeVal);
        }
        procSlave->execute();

    } while(protocolSlave->active());

    QVERIFY(procSlave->remove(protocolSlave.data()));

    QByteArray data = protocolSlave->responseData();
    QStringList headers = protocolSlave->responseHeader();
    uint16_t code = protocolSlave->responseCode();

    QCOMPARE(QString(data), "Some data for ProcessorSlave");
    QCOMPARE(headers.size(), 8);
    QCOMPARE(code, 200);

}

void HttpTest::processorMasterTest()
{
    ProtocolMasterMock *protocol = new ProtocolMasterMock(55, this);
    ProcessorMaster *processorMaster = new ProcessorMaster(55, this);
    ProcessorExecutorMock *executor = new ProcessorExecutorMock(this);
    QSignalSpy spy(processorMaster, &ProcessorMaster::protocolProcessingFinished);
    connect(processorMaster, &ProcessorMaster::addProcessorToExecution,      executor, &ProcessorExecutorMock::addProcessorToExecution     );
    connect(processorMaster, &ProcessorMaster::removeProcessorFromExecution, executor, &ProcessorExecutorMock::removeProcessorFromExecution);

    processorMaster->addProtocolToProcessing(protocol);
    QCOMPARE(executor->getProcessorMasters().size(), 1);

    processorMaster->jobExecuted();
    QVERIFY(processorMaster->isActive());

    processorMaster->removeProtocolFromProcessing(protocol);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();

    QCOMPARE(arguments.at(0).toInt(), 55);

    processorMaster->jobExecuted();
    QVERIFY(executor->getProcessorMasters().empty());
    QVERIFY(!processorMaster->isActive());
}

void HttpTest::cleanupTestCase()
{
}

