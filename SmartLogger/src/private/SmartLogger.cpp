#include "../SmartLogger.h"

QSharedPointer<QFile> SmartLogger::m_logFile;
QSharedPointer<QTextStream> SmartLogger::stream;
QTextStream SmartLogger::m_logOut;
QTextStream SmartLogger::m_console(stdout);
SmartLogger::LOGPATH SmartLogger::m_logPath;


SmartLogger::SmartLogger(const char *file, int line) : m_file(file), m_line(line)
{
    stream.reset(new QTextStream(&m_buffer, QIODevice::ReadWrite));
}

SmartLogger::~SmartLogger()
{
    (*stream.data()) << '\n';
    if(m_logPath == LOGPATH::LogToFile) {
        m_logOut << m_buffer;
        m_logOut.flush();
    } else if(m_logPath == LOGPATH::LogToStdOut) {
        m_console << m_buffer;
        m_console.flush();
    } else {
        m_logOut << m_buffer;
        m_logOut.flush();
        m_console << m_buffer;
        m_console.flush();
    }
}

void SmartLogger::initLogger(SmartLogger::LOGPATH logOption, QString logFilePath)
{
    m_logPath = logOption;

    if(m_logPath == LOGPATH::LogToFile || m_logPath == LOGPATH::LogBoth) {
        m_logFile.reset(new QFile(logFilePath));
        if(m_logFile.data()->open(QFile::WriteOnly)) {
            m_logOut.setDevice(m_logFile.data());
        } else {
            m_console << QString("ERROR! Can't open file for loggin.");
            m_console.flush();
        }
    }
}

QTextStream* SmartLogger::linker(QString logTypeStr)
{
    m_file.remove(0, m_file.lastIndexOf('/') + 1); //we need name of the file only
    (*stream.data()) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    (*stream.data()) << logTypeStr;
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
