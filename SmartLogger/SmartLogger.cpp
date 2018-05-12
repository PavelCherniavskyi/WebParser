#include "SmartLogger.h"

const QString logFilePath = "logFile.txt";
QSharedPointer<QFile> SmartLogger::m_logFile;
QSharedPointer<QTextStream> SmartLogger::stream;
QTextStream SmartLogger::logOut;
QTextStream SmartLogger::console(stdout);


SmartLogger::SmartLogger(const char *file, int line) : m_file(file), m_line(line)
{
    stream.reset(new QTextStream(&buffer, QIODevice::ReadWrite));
}

SmartLogger::~SmartLogger()
{
    (*stream.data()) << '\n';
    logOut << buffer;
    console << buffer;
    console.flush();
    logOut.flush();
}

void SmartLogger::initLogger()
{
    m_logFile.reset(new QFile(logFilePath));
    m_logFile.data()->open(QFile::Append | QFile::Text);
    logOut.setDevice(m_logFile.data());
}

QTextStream* SmartLogger::linker(QString str)
{
    m_file.remove(0, m_file.lastIndexOf('/') + 1);
    (*stream.data()) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    (*stream.data()) << str;
    (*stream.data()) << m_file << ":"<< m_line << " ";
    return stream.data();
}

QTextStream* SmartLogger::info()
{
    return linker("INF ");
}

QTextStream* SmartLogger::warn()
{
    return linker("WARN ");
}

QTextStream* SmartLogger::error()
{
    return linker("ERROR ");
}
