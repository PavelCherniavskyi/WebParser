#include "ProvisioningTest.h"

Q_DECLARE_METATYPE(Http::ProvData);

QSharedPointer<Provisioning> provisioning;

void HttpMock::init(Provisioning *prov)
{
    connect(prov, &Provisioning::onHttpDataRecieved, this, &HttpMock::OnProvDataReceived);
}

void HttpMock::OnProvDataReceived(Http::ProvData provData)
{
    QCOMPARE(provData.method, "POST");
    QCOMPARE(provData.url, "https://test.com");
    QCOMPARE(provData.timeout, static_cast<qint16>(99));
}

void ProvisioningTest::initTestCase()
{
    qRegisterMetaType<Http::ProvData>("Http::ProvData");
}

void ProvisioningTest::getProvisioningTest()
{
    Provisioning provisioning(this);
    HttpMock httpMock(this);
    httpMock.init(&provisioning);
    QVERIFY(provisioning.getProvisioning("../../WebParser/Provisioning/src/test/testProvData.xml"));
}

void ProvisioningTest::cleanupTestCase()
{

}
