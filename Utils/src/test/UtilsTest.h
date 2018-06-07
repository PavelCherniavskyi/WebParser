#include <QTest>
#include <QObject>
#include "../Job.h"
#include "../JobExecutor.h"

class JobMock : public Job
{
    Q_OBJECT
public:
    JobMock();
    bool execute() override;
    inline quint8 calls() { return callNumber; }

private:
    QVector<bool> executeCalls;
    quint8 callNumber;
};

class UtilsTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void jobTest();
    void jobExecutorTest();
    void expressServerTest();
    void cleanupTestCase();
};
