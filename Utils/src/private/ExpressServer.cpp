#include "../ExpressServer.h"
#include "../../../SmartLogger/src/SmartLogger.h"
#include <QFile>
#include <QThread>
#include <sys/types.h>
#include <signal.h>

const QString nodeJSdefaultPath = "/usr/bin/node";

ExpressServer::ExpressServer(QStringList &arguments, QObject *parent) : QObject(parent)
  , mProgPath(nodeJSdefaultPath)
  , mArguments(arguments)
  , node(new QProcess)
{
}

ExpressServer::~ExpressServer()
{
    kill(node->pid(), SIGKILL);
    node->waitForFinished(5000);
}

bool ExpressServer::startServer()
{
    if(!QFile::exists(QString(mProgPath))) {
        WARN() << "Can't find 'node' file in /usr/bin/";
        return false;
    }

    QString filePath;
    foreach (const QString arg, mArguments) {
        if(arg.contains(".js")) {
            if(!QFile::exists(arg)) {
                WARN() << "Can't find .js file.";
                return false;
            }
            filePath = arg;
            break;
        }
    }
    INFO() << "Using program path: " << mProgPath;
    INFO() << "Using file path: " << filePath;

    node->start(mProgPath, mArguments);

    node->waitForStarted(5000);
    int attempts = 5;
    for(int i = 0; i < attempts; ++i) {
        QThread::sleep(1);
        if(isServerUp()) {
            INFO() << "Express server is up";
            return true;
        } else {
            continue;
        }
    }

    return false;
}

void ExpressServer::setProgPath(QString progPath)
{
    mProgPath = progPath;
}

bool ExpressServer::isServerUp()
{
    QFile tcp6("/proc/net/tcp6");
    if(!tcp6.open(QIODevice::ReadOnly | QIODevice::Text)) {
        WARN() << "Can't open /proc/net/tcp6 ";
        return false;
    }

    bool success = false;
    QString line(tcp6.readAll());
    QStringList list = line.split(',');
    foreach (const QString str, list) {
        if(str.contains(":0BB8") && str.contains(" 0A ")) {
            success =  true;
            break;
        }
    }

    if(!success) {
        WARN() << "No open socket in /proc/net/tcp6 ";
        foreach (const QString str, list) {
            INFO() << str;
        }
    }

    tcp6.close();
    return success;
}
