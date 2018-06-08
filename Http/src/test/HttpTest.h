#include <QObject>
#include <QTest>

class HttpTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void protocolSlaveTest();
    void protocolMasterTest();
    void cleanupTestCase();
};
