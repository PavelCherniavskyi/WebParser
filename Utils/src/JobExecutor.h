#pragma once

#include <memory>
#include <string>

#include <QList>
#include <QObject>
#include <QSharedPointer>

#include <QThread>

class Job;

class JobExecutor : public QObject
{
    Q_OBJECT
public:
    explicit JobExecutor();
    ~JobExecutor();
    void execute(QSharedPointer<Job> job, bool printDebugInfo = true);
    inline int currentActiveJobs() { return mActiveJobs.size(); }

signals:
    void executeJob();

private slots:
    void executionCompleted(quint32 id);

private:
    JobExecutor(const JobExecutor & rhs) = delete;              ///< no copy
    JobExecutor &operator=(const JobExecutor &rhs) = delete;    ///< no assignment

    QList<QSharedPointer<Job>>      mActiveJobs;
    QThread                         mWorkerThread;
    int32_t                         mNewId;
    bool                            mPrintDebugInfo;
};

