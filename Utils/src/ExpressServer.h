#pragma once

#include <QObject>
#include <QProcess>
#include <QScopedPointer>

class ExpressServer : public QObject
{
    Q_OBJECT
public:
    explicit ExpressServer(QStringList &agruments, QObject *parent = nullptr);
    ~ExpressServer();
    bool startServer();
    void setProgPath(QString progPath);

signals:

public slots:

private:
    Q_DISABLE_COPY(ExpressServer)
    bool isServerUp();

    QString mProgPath;
    QStringList mArguments;
    QScopedPointer<QProcess> node;
};
