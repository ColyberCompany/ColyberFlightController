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

    float altitudeToHold_cm = 0.f;
    uint16_t altHoldThrottle = 500;

    static const uint16_t MinOutputThrottle;
    static const uint16_t MaxOutputThrottle;
    static const uint16_t MaxClimbRate_cmPerSec; // also for declining
    static const uint16_t ThrottleDeadZone;
    
    static const float ThrottleClimbRateMult; // helper var to calculate climb rate from throttle

public:
    AltHoldFlightMode(StabilizeFlightMode& stabilizeFlightMode);

    AltHoldFlightMode(const AltHoldFlightMode&) = delete;
    AltHoldFlightMode& operator=(const AltHoldFlightMode&) = delete;

    void setAltHoldPIDGains(float kP, float kI, float kD, uint16_t Imax);

    void leave() override;
    void prepare() override;

    const char* getName() override;

private:
    void flightModeLoop(Common::ControlSticks& inputOutputSticks) override;

    /**
     * @brief Integrate mapped throttle value to properly change the altitude.
     * @param throttle Raw throttle value (in range of 0 : 1000).
     */
    void updateAltitudeToHold(uint16_t throttle);
    void setAltitudeToHoldToCurrentReading();

    /**
     * @brief Calculates altitude increase in cm per second from throttle stick value.
     * @param throttle Raw throttle value (in range of 0 : 1000).
     * @return climb rate in cm per second.
     */
    static float throttleToClimbRate_cmPerSec(uint16_t throttle);
};


#endif
