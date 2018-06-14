#include "ProcessorMaster.h"
#include "../../../SmartLogger/src/SmartLogger.h"
#include "ProtocolMaster.h"

ProcessorMaster::ProcessorMaster(int32_t id, QObject *obj) : IProcessorMaster(obj)
    , mId(id)
    , mActive(false)
    , mProtocolMastersAdd()
    , mProcessorSlave(id)
    , mProtocolMasters()
{
    INFO() << "[" << mId << "] constructor";
}

ProcessorMaster::~ProcessorMaster()
{
    INFO() << "[" << mId << "] destructor";
}

ProcessorSlave *ProcessorMaster::slave()
{
    return &mProcessorSlave;
}

void ProcessorMaster::jobExecuted()
{
    if (mActive) {

        //use reverse iterator as protocols might be erased inside the loop
        for (auto rprotocol = mProtocolMasters.rbegin(); mProtocolMasters.rend() != rprotocol;) {
            (*rprotocol++)->jobExecuted();
        }

        addProtocolMasters();

        if (mProtocolMasters.empty()) {
            emit removeProcessorFromExecution(this);
            mActive = false;
        }
    }
}

void ProcessorMaster::addProtocolToProcessing(ProtocolMaster *protocol)
{
    if (protocol) {
        mProtocolMastersAdd.push_back(protocol);

        if (!mActive) {
            addProtocolMasters();

            emit addProcessorToExecution(this);
            mActive = true;

            for (auto &protocol : mProtocolMasters) {
                protocol->jobExecuted();
            }
        }
    } else {
        WARN() << "[" << mId << "] protocol is null!";
    }
}

void ProcessorMaster::removeProtocolFromProcessing(ProtocolMaster *protocol)
{
    if (protocol) {
        auto iter = std::find(mProtocolMasters.begin(), mProtocolMasters.end(), protocol);

        if (mProtocolMasters.end() != iter) {
            mProtocolMasters.erase(iter);
            static_cast<void>(mProcessorSlave.remove(protocol->slave()));
        } else {
            WARN() << "[" << mId << "] Cannot find protocol!";
        }

        emit protocolProcessingFinished(protocol->id());
    } else {
        WARN() << "[" << mId << "] protocol is null!";
    }
}

void ProcessorMaster::addProtocolMasters()
{
    for ( auto protocol : mProtocolMastersAdd ) {
        // Store pending ProtocolMaster as active ProtocolMaster.
        mProtocolMasters.push_back(protocol);

        // Connect the ProtocolSlave to the ProcessorSlave.
        static_cast<void>(mProcessorSlave.add(protocol->slave()));
    }

    // Clear list with pending ProtocolMasters as these have been
    // added to the active list now.
    mProtocolMastersAdd.clear();
}
