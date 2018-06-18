#include <QObject>
#include <QTest>
#include "DownloadManager.h"
#include "ExpressServer.h"
#include "../src/private/ProcessorMaster.h"
#include "../src/private/ProcessorExecutor.h"
#include "../src/private/ProtocolMaster.h"
#include "../src/private/ProtocolSlave.h"

class DownloadManagerMock : public IDownloadManager
{
public:
    DownloadManagerMock(QObject *obj = nullptr) : IDownloadManager(obj) {}
    void execute() override {}
    void setResponseData(const ResponseDataParams value) override;
    inline QVector<ResponseDataParams> getResponseData() { return responseHandlers; }

private:
    QVector<ResponseDataParams> responseHandlers;
};

class ProcessorMasterMock : public IProcessorMaster
{
    Q_OBJECT
public:
    ProcessorMasterMock(QObject *obj) : IProcessorMaster(obj) {}
    void jobExecuted() override;

public slots:
    void addProtocolToProcessing(IProtocolMaster *protocol) override;
    void removeProtocolFromProcessing(IProtocolMaster *protocol) override;
    inline QVector<IProtocolMaster *> getProtocolMasters() { return mProtocolMasters; }

private:
    QVector<IProtocolMaster *> mProtocolMasters;
};

class ProcessorExecutorMock : public IProcessorExecutor
{
    Q_OBJECT
public:
    ProcessorExecutorMock(QObject *obj = nullptr) : IProcessorExecutor(obj) {}
    inline QVector<ProcessorMaster *> getProcessorMasters() { return mProcessorMasters; }

public slots:
    void addProcessorToExecution(ProcessorMaster *processor) override;
    void removeProcessorFromExecution(ProcessorMaster *processor) override;
    void handleJobExecuted() override;
private:
    QVector<ProcessorMaster *> mProcessorMasters;
};

class ProtocolMasterMock : public IProtocolMaster
{
    Q_OBJECT
public:
    ProtocolMasterMock(int32_t id, QObject *obj = nullptr) : IProtocolMaster(obj)
      , mId(id)
      , mProtocolSlave(id)
    {
    }
    bool sendRequest(const QString &url, uint32_t port = 80) override;
    inline int32_t id() const override { return mId; }
    inline ProtocolSlave *slave() override { return &mProtocolSlave; }

public slots:
    void jobExecuted() override;

private:
    int32_t       mId;
    ProtocolSlave mProtocolSlave;
};

class HttpTest : public QObject
{
public:
    Q_OBJECT
private slots:
    void initTestCase();
    void protocolSlaveTest();
    void protocolMasterTest();
    void processorSlaveTest();
    void processorMasterTest();
    void cleanupTestCase();

private:
    ExpressServer expressServer;
};
