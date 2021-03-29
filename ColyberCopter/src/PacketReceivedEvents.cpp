/**
 * @file PacketReceivedEvents.cpp
 * @author Jan Wielgus
 * @date 2020-12-01
 * 
 */

#include "../Communication/PacketReceivedEvents.h"
#include "../Communication/CommData.h"
#include "../Instances/MainInstances.h"
#include "../Instances/FlightModeInstances.h"

using namespace PacketReceivedEvents;


void Steering::execute()
{
    // VirtualPilot get data directly from received variables.

    // Add other actions here if needed.
}


void FlightModeChange::execute()
{
    uint8_t newFlightModeType = commData.flightMode;
    Instance::virtualPilot.setFlightMode((Enums::FlightModeTypes)newFlightModeType);
}


void PIDTuning::execute()
{
    using Assemble::FlightModes::stabilizeFlightMode;

    switch (commData.pidTuning.tunedController_ID)
    {
        case 0: // leveling
            stabilizeFlightMode.setLevelingXPIDGains(commData.pidTuning.kP,
                                                     commData.pidTuning.kI,
                                                     commData.pidTuning.kD,
                                                     commData.pidTuning.iMax);

            stabilizeFlightMode.setLevelingYPIDGains(commData.pidTuning.kP,
                                                     commData.pidTuning.kI,
                                                     commData.pidTuning.kD,
                                                     commData.pidTuning.iMax);
            break;
        
        case 1: // yaw  
            stabilizeFlightMode.setHeadingHoldPIDGains(commData.pidTuning.kP,
                                                       commData.pidTuning.kI,
                                                       commData.pidTuning.kD,
                                                       commData.pidTuning.iMax);
    }
}


// ... (implementations of other received events)

