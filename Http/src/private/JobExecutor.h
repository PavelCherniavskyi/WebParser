#pragma once

#include <memory>
#include <string>

#include <QList>
#include <QObject>
#include <QSharedPointer>

#include <QThread>

class HttpJob;

/*! \brief class JobExecutor controls execution of different Jobs which are derived from class Job.
Usage example:

class SomeJob
    : public Job
{
    int32_t mCurrValue;
    int32_t mFinalValue;

public:
    SomeJob(int32_t initialValue, int32_t finalValue, const std::string &name = "SomeJob")
        : Job(name)
        , mCurrValue(initialValue)
        , mFinalValue(finalValue)
    {
        LOG(info) << "[" << name << "] Constructor";
    }

    ~SomeJob()
    {
        LOG(info) << "[" << id() << "] Destructor";
    }

    bool execute() override
    {
        LOG(info) << "[" << id() << "] Current value:" << mCurrValue;

        if (mCurrValue != mFinalValue) {
            mCurrValue++;
            return true;
        } else {
            return false;
        }
    }
};

void main()
{
    Dumm::JobExecutor jobExecutor;

    jobExecutor.execute(QSharedPointer<SomeJob>(new SomeObj(1, 10), &QObject::deleteLater));
    jobExecutor.execute(QSharedPointer<SomeJob>(new SomeObj(31, 40, "Second job"), &QObject::deleteLater));
    jobExecutor.execute(QSharedPointer<SomeJob>(new SomeObj(51, 60, "Third job"), &QObject::deleteLater));
}
*/

class JobExecutor
    : public QObject
{
    Q_OBJECT

public:
    /*! \brief Constructor */
    explicit JobExecutor();

    /*! \brief Destructor */
    ~JobExecutor();

    /*! \brief Executes job in the workerthread
        \param job Job to be executed
        \param printDebugInfo false if no need to print debug info*/
    void execute(QSharedPointer<HttpJob> job, bool printDebugInfo = true);

signals:
    /*! \brief Starts Job execution */
    void executeJob();

private slots:
    /*! \brief Job sends a signal to this slot when it is completed */
    void executionCompleted(int32_t id);

private:
    JobExecutor(const JobExecutor & rhs) = delete;              ///< no copy
    JobExecutor &operator=(const JobExecutor &rhs) = delete;    ///< no assignment

    QList<QSharedPointer<HttpJob>>  mActiveJobs;
    QThread                         mWorkerThread;
    int32_t                         mNewId;
    bool                            mPrintDebugInfo;
};

