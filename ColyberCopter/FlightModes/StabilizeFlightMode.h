/**
 * @file StabilizeFlightMode.h
 * @author Jan Wielgus
 * @brief Stabilize flight mode concrete class.
 * @date 2020-08-02
 * 
 */

#ifndef STABILIZEFLIGHTMODE_H
#define STABILIZEFLIGHTMODE_H

#include "FlightMode.h"
#include <PID.h>


class StabilizeFlightMode : public FlightMode
{
    PID levelingXPID;
    PID levelingYPID;
    PID headingHoldPID;

    float headingToHold = 0; // [deg] (0 : 359.99...)
    float headingError = 0; // [deg] (-180 : 180)


public:
    StabilizeFlightMode();
    StabilizeFlightMode(const StabilizeFlightMode&) = delete;
    StabilizeFlightMode& operator=(const StabilizeFlightMode&) = delete;

    void setLevelingXPIDGains(float kP, float kI, float kD, uint16_t Imax);
    void setLevelingYPIDGains(float kP, float kI, float kD, uint16_t Imax);
    void setHeadingHoldPIDGains(float kP, float kI, float kD, uint16_t Imax);

    void leave() override;
    void prepare() override;

    const char* getName() override;
    
private:
    void flightModeLoop(ControlSticks& inputOutputSticks) override;

    void updateLeveling(ControlSticks& inputOutputSticks);
    void updateHeadingHolding(ControlSticks& inputOutputSticks);
    void updateHeadingToHold(int16_t yawStick);
    void calculateHeadingError();
    void setHeadingToHoldToCurrentReading();

    /**
     * @brief Makes value from parameter to be from range 0 - 360 deg.
     * Don't constrain value but makes modulo on float.
     * @param headingToCorrect Heading angle to correct.
     * @return Heading from range 0 - 360 deg.
     */
    static float correctHeading(float headingToCorrect);
};


#endif
