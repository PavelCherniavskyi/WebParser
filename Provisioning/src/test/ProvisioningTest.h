#include <QTest>
#include <iostream>
#include <unistd.h>

#include "../Provisioning.h"

class HttpMock : public QObject
{
    Q_OBJECT
public:
    HttpMock(QObject *obj) : QObject(obj) {}
    void init(Provisioning* prov);

private slots:
    void OnProvDataReceived(Http::ProvData provData);
};

class ProvisioningTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void getProvisioningTest();
    void cleanupTestCase();
};
