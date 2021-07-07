/**
 * @file AltHoldFlightMode.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief AltHold flight mode concrete class.
 * @date 2021-02-24
 * 
 */

#ifndef ALTHOLDFLIGHTMODE_H
#define ALTHOLDFLIGHTMODE_H

#include "FlightMode.h"
#include "StabilizeFlightMode.h"
#include <PID.h>


class AltHoldFlightMode : public FlightMode
{
    PID altitudeHoldPID;

    float altitudeToHold_cm = 0;
    float altitudeError_cm = 0;

    static const uint16_t MinOutputThrottle;
    static const uint16_t MaxOutputThrottle;
    static const uint16_t MaxClimbRate_cmPerSec; // also for declining
    
    static const float ThrottleMultiplier;

public:
    AltHoldFlightMode(StabilizeFlightMode& stabilizeFlightMode);

    AltHoldFlightMode(const AltHoldFlightMode&) = delete;
    AltHoldFlightMode& operator=(const AltHoldFlightMode&) = delete;

    void setAltHoldPIDGains(float kP, float kI, float kD, uint16_t Imax);

    void leave() override;
    void prepare() override;

    const char* getName() override;

private:
    void flightModeLoop(ControlSticks& inputOutputSticks) override;

    void updateAltitudeHolding(ControlSticks& inputOutputSticks);
    void updateAltitudeToHold(uint16_t throttle);
    void calculateAltitudeError();
    void setAltitudeToHoldToCurrentReading();

    static float throttleToClimbRate_cmPerSec(uint16_t throttle);
};


#endif
