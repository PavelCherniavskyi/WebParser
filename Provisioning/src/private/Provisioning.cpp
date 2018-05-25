#include <QXmlQuery>
#include <QtXml>
#include <QDebug>

#include "../Provisioning.h"

Provisioning::Provisioning(QObject *obj) : QObject(obj)
{

}

bool Provisioning::getProvisioning()
{


    QFile provData("../WebParser/ProvData.xml");
    if(provData.open(QIODevice::ReadOnly)) {
        qDebug() << "Opened file susseccfully";

        QXmlQuery query;
        query.setFocus(&provData);

        //Provisioning for Http
        Http::ProvData httpProvData;
        query.setQuery("provdata/http/url/text()");
        query.evaluateTo(&httpProvData.url);
        httpProvData.url.remove(httpProvData.url.size() - 1, 1); //to cut '\n' at the end.

        query.setQuery("provdata/http/method/text()");
        query.evaluateTo(&httpProvData.method);
        httpProvData.method.remove(httpProvData.method.size() - 1, 1);

        query.setQuery("provdata/http/timeout/text()");
        QString temp;
        query.evaluateTo(&temp);
        httpProvData.timeout = static_cast<quint16>(temp.toInt());

        //Provisioning for SmartLogger
        SmartLogger::ProvData smartProvData;
        query.setQuery("provdata/smartlogger/logtofile/text()");
        query.evaluateTo(&temp);
        bool logTofile = (temp.toUpper() == "TRUE");

        query.setQuery("provdata/smartlogger/logtostdout/text()");
        query.evaluateTo(&temp);
        bool logToStdout = (temp.toUpper() == "TRUE");

        if(logTofile && logToStdout) {
            smartProvData.logWay = SmartLogger::LOGWAY::LogBoth;
        } else if(logTofile) {
            smartProvData.logWay = SmartLogger::LOGWAY::LogToFile;
        } else {
            smartProvData.logWay = SmartLogger::LOGWAY::LogToStdOut;
        }

        query.setQuery("provdata/smartlogger/filePath/text()");
        query.evaluateTo(&smartProvData.logFilePath);
        smartProvData.logFilePath.remove(smartProvData.logFilePath.size() - 1, 1);

        provData.close();

        emit onHttpDataRecieved(httpProvData);
        emit onSmartLoggerDataRecieved(smartProvData);

    } else {
        qDebug() << "Can't open provData.xml";
    }

    return true;
}
