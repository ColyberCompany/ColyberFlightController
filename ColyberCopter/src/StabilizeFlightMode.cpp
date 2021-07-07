/**
 * @file StabilizeFlightMode.cpp
 * @author Jan Wielgus
 * @date 2020-08-02
 * 
 */

#include "../FlightModes/StabilizeFlightMode.h"
#include "../Common/Constants.h"
#include "../Instances/MainInstances.h"
#include "../config.h"

using Enums::FlightModeTypes;
using Consts::RoundAngle;;
using Consts::StraightAngle;

using Config::LevelingPID_kP;
using Config::LevelingPID_kI;
using Config::LevelingPID_kD;
using Config::LevelingPID_IMax;
using Config::HeadHoldPID_kP;
using Config::HeadHoldPID_kI;
using Config::HeadHoldPID_kD;
using Config::HeadHoldPID_IMax;


StabilizeFlightMode::StabilizeFlightMode()
    : FlightMode(FlightModeTypes::STABILIZE, nullptr),
    levelingXPID(Config::MainInterval_s),
    levelingYPID(Config::MainInterval_s),
    headingHoldPID(Config::MainInterval_s)
{
    levelingXPID.setGains(LevelingPID_kP, LevelingPID_kI, LevelingPID_kD, LevelingPID_IMax);
    levelingYPID.setGains(LevelingPID_kP, LevelingPID_kI, LevelingPID_kD, LevelingPID_IMax);
    headingHoldPID.setGains(HeadHoldPID_kP, HeadHoldPID_kI, HeadHoldPID_kD, HeadHoldPID_IMax);
}


void StabilizeFlightMode::setLevelingXPIDGains(float kP, float kI, float kD, uint16_t Imax)
{
    levelingXPID.setGains(kP, kI, kD, Imax);
}


void StabilizeFlightMode::setLevelingYPIDGains(float kP, float kI, float kD, uint16_t Imax)
{
    levelingYPID.setGains(kP, kI, kD, Imax);
}


void StabilizeFlightMode::setHeadingHoldPIDGains(float kP, float kI, float kD, uint16_t Imax)
{
    headingHoldPID.setGains(kP, kI, kD, Imax);
}


void StabilizeFlightMode::leave()
{
    levelingXPID.reset();
    levelingYPID.reset();
    headingHoldPID.reset();
}


void StabilizeFlightMode::prepare()
{
    setHeadingToHoldToCurrentReading();
}


const char* StabilizeFlightMode::getName()
{
    return "stabilize";
}


void StabilizeFlightMode::flightModeLoop(ControlSticks& inputOutputSticks)
{
    if (inputOutputSticks.getThrottle() < 100) // TODO: think if this could be checked in a better way then just throttle threshold
        return;

    updateLeveling(inputOutputSticks);
    updateHeadingHolding(inputOutputSticks);
}


void StabilizeFlightMode::updateLeveling(ControlSticks& inputOutputSticks)
{
    float finalPitch = inputOutputSticks.getPitch() / 10.f; // TODO: make that max tilt angle can be set
    float finalRoll = inputOutputSticks.getRoll() / 10.f;
    vector3Float angles = Instance::ahrs.getAngles_deg();

    inputOutputSticks.setPitch(levelingXPID.update(finalPitch, angles.x) + 0.5f);
    inputOutputSticks.setRoll(levelingYPID.update(finalRoll, angles.y) + 0.5f);
}


void StabilizeFlightMode::updateHeadingHolding(ControlSticks& inputOutputSticks)
{
    updateHeadingToHold(inputOutputSticks.getYaw());
    calculateHeadingError();
    
    inputOutputSticks.setYaw(headingHoldPID.update(headingError));
}


void StabilizeFlightMode::updateHeadingToHold(int16_t yawStick)
{
    headingToHold -= ((float)(yawStick / 2.f) * Config::MainInterval_s);
    headingToHold = correctHeading(headingToHold);
}


void StabilizeFlightMode::calculateHeadingError()
{
    headingError = headingToHold - Instance::ahrs.getHeading_deg();

    if (headingError > StraightAngle)
        headingError -= RoundAngle;
    else if (headingError < -StraightAngle)
        headingError += RoundAngle;
}


void StabilizeFlightMode::setHeadingToHoldToCurrentReading()
{
    headingToHold = Instance::ahrs.getHeading_deg();
}


float StabilizeFlightMode::correctHeading(float headingToCorrect)
{
    if (headingToCorrect >= RoundAngle)
    {
        headingToCorrect -= RoundAngle;
        while (headingToCorrect >= RoundAngle)
            headingToCorrect -= RoundAngle;
    }
    else if (headingToCorrect < 0.f)
    {
        headingToCorrect += RoundAngle;
        while (headingToCorrect < 0.f)
            headingToCorrect += RoundAngle;
    }
    
    return headingToCorrect;
}
