#include <QXmlQuery>
#include <QXmlStreamReader>
#include <QtXml>
#include <QDebug>

#include "../Provisioning.h"

const QString provDataPathDefault = "../WebParser/ProvData.xml";

Provisioning::Provisioning(QObject *obj) : QObject(obj)
{

}

Provisioning::~Provisioning()
{

}

bool Provisioning::getProvisioning(QString path)
{
    if (path.isEmpty()) {
        path = provDataPathDefault;
    }

    QFile provData(path);
    if(provData.open(QIODevice::ReadOnly)) {

        //Provisioning for Http
        DownloadManager::ProvData downloadProvData;
        QXmlStreamReader xml(&provData);
        QList<QString> urls;
        while (!xml.atEnd() && !xml.hasError()) //parsing urls in cycle
        {
            if (xml.isStartElement())
            {
                if(xml.name() == "urls") {
                    xml.readNext();
                    while (xml.name() != "urls")
                    {
                        if(xml.isStartElement()) {
                            urls.push_back(xml.readElementText());
                        }
                        xml.readNext();
                    }
                }
            }
            xml.readNext();
        }
        if (xml.hasError()) {
            WARN() << "XML reader error: " << xml.errorString()
                   << "Line:" << static_cast<int>(xml.lineNumber())
                   << "Column:" << static_cast<int>(xml.columnNumber());
            return false;
        }
        downloadProvData.urls = urls;

        QXmlQuery query;
        QString temp;

        provData.seek(0);
        query.setFocus(&provData);

        query.setQuery("provdata/http/settings/phones/text()");
        query.evaluateTo(&temp);
        downloadProvData.parseTypes.insert(Parser::Phones, (temp.trimmed().toUpper() == "TRUE"));

        query.setQuery("provdata/http/settings/emails/text()");
        query.evaluateTo(&temp);
        downloadProvData.parseTypes.insert(Parser::Emails, (temp.trimmed().toUpper() == "TRUE"));

        query.setQuery("provdata/http/settings/timeout/text()");
        query.evaluateTo(&temp);
        downloadProvData.timeout = static_cast<quint16>(temp.toInt());

        //Provisioning for SmartLogger
        SmartLogger::ProvData smartProvData;
        query.setQuery("provdata/smartlogger/logtofile/text()");
        query.evaluateTo(&temp);
        bool logTofile = (temp.trimmed().toUpper() == "TRUE");

        query.setQuery("provdata/smartlogger/logtostdout/text()");
        query.evaluateTo(&temp);
        bool logToStdout = (temp.trimmed().toUpper() == "TRUE");

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

        emit onDownloadMngrDataRecieved(downloadProvData);
        emit onSmartLoggerDataRecieved(smartProvData);

    } else {
        WARN() << "Can't open provData.xml";
        return false;
    }

    return true;
}
