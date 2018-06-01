#pragma once

#include <QObject>
#include <memory>
#include <QVector>

class ProcessorSlave;

/*! \brief this class provides performing of all the multi/easy handles. Shoud be run in a worker thread. */
class HttpJob : public QObject //: public Job
{
    Q_OBJECT
public:
    HttpJob(const QVector<ProcessorSlave *>& processorSlaves, const QString &name = "HttpJob");
    ~HttpJob();
    bool execute();
    int32_t id() const;
    void setId(int32_t id);
    const QString &name();

signals:
    void continueExecution();
    void jobExecuted();
    void executionCompleted(int32_t id);

public slots:
    void executeJob();

private:
    void curlSelect();
    QVector<ProcessorSlave *> mProcessorSlaves;
    int32_t     mId;
    QString     mName;
};

