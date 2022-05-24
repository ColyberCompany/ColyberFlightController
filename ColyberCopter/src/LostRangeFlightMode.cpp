/**
 * @file LostRangeFlightMode.cpp
 * @author Antoni Wielgus
 * @date 2021-08-28
 * 
 */

#include "../FlightModes/LostRangeFlightMode.h"
#include "../Instances/MotorsInstance.h"


LostRangeFlightMode::LostRangeFlightMode(int _whenTurnOffMotorsInMiliseconds = 3000)
{
    whenTurnOffMotorsInMiliseconds = _whenTurnOffMotorsInMiliseconds;
}

void LostRangeFlightMode::prepare()
{
    startTime = millis();
}

void LostRangeFlightMode::leave()
{

}

const char* LostRangeFlightMode::getName()
{
    return "LostRange";
}

void LostRangeFlightMode::flightModeLoop(Common::ControlSticks& inputOutputSticks)
{
    if(millis() >= startTime + whenTurnOffMotorsInMiliseconds)
        Instance::motors.setState(Enums::StateType::Disabled);
    else
        Instance::motors.setState(Enums::StateType::Enabled);
}