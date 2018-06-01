#include "ProcessorMaster.h"
#include "../../../SmartLogger/src/SmartLogger.h"
#include "ProtocolMaster.h"

// NOTE: being in active state the workerthread processes the slave part (ProcessorSlave) of this ProcessorMaster.
// In Active state access to the ProcessorSlave is not possible without race condition.

//--------------------------------------------------------------------------------------------------
ProcessorMaster::ProcessorMaster(int32_t id)
//--------------------------------------------------------------------------------------------------
    : mId(id)
    , mActive(false)
    , mProcessorSlave(id)
{
    INFO() << "[" << mId << "] constructor";
}

//--------------------------------------------------------------------------------------------------
ProcessorMaster::~ProcessorMaster()
//--------------------------------------------------------------------------------------------------
{
    INFO() << "[" << mId << "] destructor";
}

//--------------------------------------------------------------------------------------------------
ProcessorSlave *ProcessorMaster::slave()
//--------------------------------------------------------------------------------------------------
{
    return &mProcessorSlave;
}

//--------------------------------------------------------------------------------------------------
void ProcessorMaster::jobExecuted()
//--------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------
void ProcessorMaster::addProtocolToProcessing(ProtocolMaster *protocol)
//--------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------
void ProcessorMaster::removeProtocolFromProcessing(ProtocolMaster *protocol)
//--------------------------------------------------------------------------------------------------
{
    if (protocol) {
        ///@todo: maybe add assertion whether ProcessorExecutor is not active
        ///       no workerthread run is currently active.

        // The client of ProcessorMaster called the 'remove()' function
        // while the ProcessorSlave is being executed within the workerthread.
        // That is not allowed and must be prevented. The client is
        // responsible to synchronize to the workerthread activity and call
        // 'remove()' only when the workerthread is not running.
        // ASSERT( false ); todo: enable again

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

//--------------------------------------------------------------------------------------------------
void ProcessorMaster::addProtocolMasters()
//--------------------------------------------------------------------------------------------------
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
