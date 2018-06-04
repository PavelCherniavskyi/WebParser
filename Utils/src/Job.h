#pragma once

#include <QObject>
#include <string>

/*! \brief class Job is used as base class for all the jobs which are controlled by JobExecutor.
  *                           see usage example in description of class JobExecutor*/
class Job : public QObject
{
    Q_OBJECT
public:
    explicit Job(QObject *obj = 0);
    virtual ~Job();
    void setId(int32_t id);
    int32_t id() const;
    const QString &name();
    virtual bool execute() = 0;

signals:
    void continueExecution();
    void executionCompleted(int32_t id);
    void jobExecuted();

public slots:
    void executeJob();

private:
    Job(const Job & rhs) = delete;              ///< no copy
    Job &operator=(const Job &rhs) = delete;    ///< no assignment

    int32_t     mId;
    bool        mPrintDebugInfo;
};
