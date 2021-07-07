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
    // TODO: show result of setting the flight mode in a debug message (maybe show debug message in VirtualPilot)

    // TODO: log flight mode setting result (maybe log flight mode change in VirtualPilot class)
}


void PIDTuning::execute()
{
    using Assemble::FlightModes::stabilizeFlightMode;
    using Instance::debMes;

    debMes.showMessage("Got new PID. ID:");
    debMes.showMessage(commData.pidTuning.tunedController_ID);
    debMes.showMessage("kP, kI, kD, iMax:");
    debMes.showMessage(commData.pidTuning.kP * 100);
    debMes.showMessage(commData.pidTuning.kI * 100);
    debMes.showMessage(commData.pidTuning.kD * 100);
    debMes.showMessage(commData.pidTuning.iMax);
    

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

