#pragma once

#include <QObject>
#include <memory>
#include <QVector>
#include "../../../Utils/src/Job.h"

class ProcessorSlave;

/*! \brief this class provides performing of all the multi/easy handles. Shoud be run in a worker thread. */
class HttpJob : public Job
{
    Q_OBJECT
public:
    HttpJob(ProcessorSlave *processorSlave);
    ~HttpJob();
    bool execute();
    int32_t id() const;
    void setId(int32_t id);

private:
    void curlSelect();
    ProcessorSlave *mProcessorSlave;
    int32_t     mId;
};

