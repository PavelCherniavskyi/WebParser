#include "SmartLogger.h"

const QString logFilePath = "logFile.txt";
QSharedPointer<QFile> m_logFile;


void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QTextStream logOut(m_logFile.data());
    static QTextStream console(stdout);
    QString str;
    QTextStream buffer(&str, QIODevice::ReadWrite);
    buffer << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    switch (type)
    {
    case QtInfoMsg:     buffer << "INF "; break;
    case QtDebugMsg:    buffer << "DBG "; break;
    case QtWarningMsg:  buffer << "WRN "; break;
    case QtCriticalMsg: buffer << "CRT "; break;
    case QtFatalMsg:    buffer << "FTL "; break;
    }

    buffer << QT_MESSAGELOG_FILE << ": ";
    buffer << QT_MESSAGELOG_LINE << " ";

    buffer << msg << endl;
    buffer.flush();

    logOut << str;
    console << str;
    console.flush();
    logOut.flush();
}

void SmartLogger::initLogger()
{
    m_logFile.reset(new QFile(logFilePath));
    m_logFile.data()->open(QFile::Append | QFile::Text);
    qInstallMessageHandler(messageHandler);
}
