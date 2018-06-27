#include "../SmartLogger.h"
#include "../Provisioning/src/Provisioning.h"
#include <dlt.h>

DLT_DECLARE_CONTEXT(smart_logger)

QScopedPointer<QFile> SmartLogger::m_logFile;
QScopedPointer<QTextStream> SmartLogger::stream;
QTextStream SmartLogger::m_logOut;
QTextStream SmartLogger::m_console(stdout);
SmartLogger::LOGWAY SmartLogger::m_logWay           = SmartLogger::LOGWAY::LogToStdOut;
QString SmartLogger::m_logFilePath                  = "log_file.txt";
QMutex SmartLogger::mutex;
bool SmartLogger::m_isDLTEnabled;
QProcess *SmartLogger::m_dlt_daemon_proc;

SmartLogger::SmartLogger(const char *file, const char *function, int line)
    : m_file(file)
    , m_line(line)
    , m_function(function)
{
    mutex.lock();
    stream.reset(new QTextStream(&m_buffer, QIODevice::ReadWrite));
}

SmartLogger::~SmartLogger()
{
    (*stream.data()) << '\n';
    if(m_logWay == LOGWAY::LogToFile) {
        m_logOut << m_buffer;
        m_logOut.flush();
    } else if(m_logWay == LOGWAY::LogToStdOut) {
        m_console << m_buffer;
        m_console.flush();
    } else {
        m_logOut << m_buffer;
        m_logOut.flush();
        m_console << m_buffer;
        m_console.flush();
    }
    mutex.unlock();
}

void SmartLogger::init(Provisioning *prov)
{
    connect(prov, &Provisioning::onSmartLoggerDataRecieved, OnProvDataReceived);
}

void SmartLogger::OnProvDataReceived(SmartLogger::ProvData provData)
{
    INFO() << "Provisionig received: " << EnumToQStr(provData.logWay);

    if(provData.logWay != LOGWAY::NONE) {
        m_logWay = provData.logWay;
    }
    if(!provData.logFilePath.isEmpty()) {
        m_logFilePath = provData.logFilePath;
    }

    if(m_logWay == LOGWAY::LogToFile || m_logWay == LOGWAY::LogBoth) {
        m_logFile.reset(new QFile(m_logFilePath));
        if(m_logFile.data()->open(QFile::WriteOnly | QIODevice::Truncate)) {
            m_logOut.setDevice(m_logFile.data());
        } else {
            m_console << QString("ERROR! Can't open file for loggin.");
            m_console.flush();
        }
    }

    m_isDLTEnabled = provData.isDLTEnabled;
    if(m_isDLTEnabled){
        startDLTDeamon();
    }
}

QTextStream* SmartLogger::linker(QString logTypeStr)
{
    m_file.remove(0, m_file.lastIndexOf('/') + 1); //we need name of the file only
    m_file.remove(m_file.lastIndexOf('.'), m_file.size()); //we need name of the class only
    (*stream.data()) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    (*stream.data()) << logTypeStr;
    (*stream.data()) << m_file << "::"<< m_function << "  ";
    return stream.data();
}

void SmartLogger::startDLTDeamon()
{
//    QFile tcp6("/proc/net/tcp6");
//    if(!tcp6.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        WARN() << "Can't open /proc/net/tcp6 ";
//        return false;
//    }

//    m_dlt_daemon_proc = new QProcess();
//    QStringList arguments;
//    m_dlt_daemon_proc->start("/usr/local/bin/dlt-daemon", arguments);
//    m_dlt_daemon_proc->waitForStarted(5000);
    INFO();


    DLT_REGISTER_APP("LOG","Test Application for Logging");
    DLT_REGISTER_CONTEXT(smart_logger,"MYCT", "My Context");

    DLT_LOG(smart_logger, DLT_LOG_INFO, DLT_STRING("Hello world"),DLT_INT(568));
    sleep(1);

    DLT_UNREGISTER_CONTEXT(smart_logger);
    DLT_UNREGISTER_APP();
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
