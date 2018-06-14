#include <QObject>
#include <QTest>
#include "DownloadManager.h"
#include "../src/private/ProcessorMaster.h"
#include "../src/private/ProcessorExecutor.h"

class DownloadManagerMock : public IDownloadManager
{
public:
    DownloadManagerMock(QObject *obj = 0) : IDownloadManager(obj) {}
    void execute() {}
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
    void addProtocolToProcessing(ProtocolMaster *protocol) override;
    void removeProtocolFromProcessing(ProtocolMaster *protocol) override;
    inline QVector<ProtocolMaster *> getProtocolMasters() { return mProtocolMasters; }

private:
    QVector<ProtocolMaster *> mProtocolMasters;
};

class ProcessorExecutorMock : public IProcessorExecutor
{
    Q_OBJECT
public:
    ProcessorExecutorMock(QObject *obj = 0) : IProcessorExecutor(obj) {}

public slots:
    void addProcessorToExecution(ProcessorMaster *processor) override;
    void removeProcessorFromExecution(ProcessorMaster *processor) override;
    void handleJobExecuted() override;
};

class HttpTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void protocolSlaveTest();
    void protocolMasterTest();
    void cleanupTestCase();
};
