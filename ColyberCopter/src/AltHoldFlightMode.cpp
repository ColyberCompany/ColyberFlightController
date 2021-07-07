/**
 * @file AltHoldFlightMode.cpp
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @date 2021-02-24
 * 
 */

#include "../FlightModes/AltHoldFlightMode.h"
#include "../Instances/MainInstances.h"
#include "../config.h"

using Enums::FlightModeTypes;


const uint16_t AltHoldFlightMode::MinOutputThrottle = 300;
const uint16_t AltHoldFlightMode::MaxOutputThrottle = 700;
const uint16_t AltHoldFlightMode::MaxClimbRate_cmPerSec = 100; // also for declining

const float AltHoldFlightMode::ThrottleMultiplier = MaxClimbRate_cmPerSec / 500.f;


AltHoldFlightMode::AltHoldFlightMode(StabilizeFlightMode& stabilizeFlightMode)
    : FlightMode(FlightModeTypes::ALT_HOLD, &stabilizeFlightMode),
    altitudeHoldPID(Config::MainInterval_s)
{
    setAltHoldPIDGains(Config::AltHoldPID_kP,
                       Config::AltHoldPID_kI,
                       Config::AltHoldPID_kD,
                       Config::AltHoldPID_IMax);
}


void AltHoldFlightMode::setAltHoldPIDGains(float kP, float kI, float kD, uint16_t Imax)
{
    altitudeHoldPID.setGains(kP, kI, kD, Imax);
}


void AltHoldFlightMode::leave()
{
    altitudeHoldPID.reset();
}


void AltHoldFlightMode::prepare()
{
    setAltitudeToHoldToCurrentReading();
}


const char* AltHoldFlightMode::getName()
{
    return "alt_hold";
}


void AltHoldFlightMode::flightModeLoop(ControlSticks& inputOutputSticks)
{
    if (inputOutputSticks.getThrottle() < 80) // TODO: think if this could be checked in a better way then just throttle threshold
        return;
    
    updateAltitudeHolding(inputOutputSticks);
}


void AltHoldFlightMode::updateAltitudeHolding(ControlSticks& inputOutputSticks)
{
    updateAltitudeToHold(inputOutputSticks.getThrottle());
    calculateAltitudeError();

    int16_t outputThrottle = altitudeHoldPID.update(altitudeError_cm);
    outputThrottle = constrain(outputThrottle, MinOutputThrottle, MaxOutputThrottle);
    inputOutputSticks.setThrottle(outputThrottle);
}


void AltHoldFlightMode::updateAltitudeToHold(uint16_t throttle)
{
    altitudeToHold_cm += throttleToClimbRate_cmPerSec(throttle) * Config::MainInterval_s;
}


void AltHoldFlightMode::calculateAltitudeError()
{
    altitudeError_cm = altitudeToHold_cm - (Instance::ahrs.getAltitude_m() * 100.f);
}


void AltHoldFlightMode::setAltitudeToHoldToCurrentReading()
{
    altitudeToHold_cm = Instance::ahrs.getAltitude_m() * 100.f;
}


float AltHoldFlightMode::throttleToClimbRate_cmPerSec(uint16_t throttle)
{
    int16_t centeredThrottle = throttle - Config::ThrottleStickCenter;
    return centeredThrottle * ThrottleMultiplier;
}
