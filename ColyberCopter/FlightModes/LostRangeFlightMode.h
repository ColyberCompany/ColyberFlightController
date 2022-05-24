/**
 * @file LostRangeMode.h
 * @author Antoni Wielgus
 * @brief 
 * @date 2021-08-28
 * 
 */

#ifndef LOSTRANGEMODE_H
#define LOSTRANGEMODE_H

#include "FlightMode.h"


class LostRangeFlightMode : public FlightMode
{
    unsigned long startTime;
    int whenTurnOffMotorsInMiliseconds;

public:
    LostRangeFlightMode(int _whenTurnOffMotorsInMiliseconds);

    void leave() override;
    void prepare() override;
    const char* getName();

private:
    void flightModeLoop(Common::ControlSticks& inputOutputSticks) override;

};


#endif