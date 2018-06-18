#pragma once

#include <QObject>
#include <string>

/*! \brief class Job is used as base class for all the jobs which are controlled by JobExecutor.
  *                           see usage example in description of class JobExecutor*/
class Job : public QObject
{
    Q_OBJECT
public:
    explicit Job(QObject *obj = nullptr);
    virtual ~Job();
    void setId(int id);
    int id() const;
    const QString &name();
    virtual bool execute() = 0;

signals:
    void continueExecution();
    void executionCompleted(int id);
    void jobExecuted();

public slots:
    void executeJob();

private:
    Job(const Job & rhs) = delete;              ///< no copy
    Job &operator=(const Job &rhs) = delete;    ///< no assignment

    int     mId;
    bool        mPrintDebugInfo;
};
