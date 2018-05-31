#include "ProvisioningTest.h"

Q_DECLARE_METATYPE(Http::ProvData);

QSharedPointer<Provisioning> provisioning;

void HttpMock::init(Provisioning *prov)
{
    connect(prov, &Provisioning::onHttpDataRecieved, this, &HttpMock::OnProvDataReceived);
}

void HttpMock::OnProvDataReceived(Http::ProvData provData)
{
    QCOMPARE(provData.urls.size(), 3);
    for(uint i = 0; i < provData.urls.size(); ++i) {
        QString str("https://test.com");
        str += QString::number(i);
        QCOMPARE(provData.urls[i], str);
    }
    QCOMPARE(provData.timeout, 99);
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
