/**
 * @file GPS.h
 * @author Jan Wielgus
 * @brief Base class for all GPSes.
 * @date 2021-08-12
 */

#ifndef __GPS_H__
#define __GPS_H__

#include "Sensor.h"
#include <cstdint>


class GPS : public Sensor
{
public:
    GPS()
        : Sensor(Sensor::SensorTypes::GPS)
    {
    }

    virtual ~GPS() {}

    virtual double getLatitude_deg() = 0;
    virtual double getLongitude_deg() = 0;
    virtual uint32_t getSattelitesAmt() = 0;
    virtual float getSpeed_kmph() = 0;
};


#endif // __GPS_H__
