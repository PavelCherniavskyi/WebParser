#include <QTest>
#include <QObject>
#include "../Job.h"

class JobMock : public Job
{
    Q_OBJECT
public:
    JobMock();
    bool execute() override;
    inline int numberOfCalls() { return executeCalls.size(); }

public slots:
    void executionCompleted(int32_t id);

private:
    QVector<bool> executeCalls;
};

class UtilsTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void jobTest();
    void jobExecutorTest();
    void cleanupTestCase();
};
