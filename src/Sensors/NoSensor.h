/**
 * @file NoSensor.h
 * @author Jan Wielgus
 * @brief Instance put instead of the particular sensor instance
 * if this sensor is not available.
 * @date 2020-12-01
 * 
 */

#ifndef NOSENSOR_H
#define NOSENSOR_H

#include "Base/Sensor.h"


class NoSensor : public Sensor
{
public:
    NoSensor()
        : Sensor(Sensor::SensorTypes::NO_SENSOR)
    {}

    const char* getName() override
    {
        return "no sensor";
    }

private:
    bool init_priv() override
    {
        return false;
    }
};


#endif
