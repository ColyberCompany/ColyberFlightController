/**
 * @file Barometer.h
 * @author Jan Wielgus
 * @brief Base class for all barometers (pressure sensors).
 * @date 2021-08-12
 */

#ifndef __BAROMETER_H__
#define __BAROMETER_H__

#include "Sensor.h"


class Barometer : public Sensor
{
public:
    Barometer()
        : Sensor(Sensor::SensorTypes::BAROMETER)
    {
    }

    virtual ~Barometer() {}

    /**
     * @brief Current instance as barometer.
     */
    Barometer& barometer() {
        return *this;
    }

    virtual float getPressure_hPa() = 0;
};


#endif // __BAROMETER_H__
