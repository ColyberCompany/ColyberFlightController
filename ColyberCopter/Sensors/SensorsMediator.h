/**
 * @file SensorsMediator.h
 * @author Jan Wielgus
 * @brief This class is used to exchange sensor readings
 * between sensors and rest of the project.
 * @date 2020-11-25
 * 
 */


#ifndef SENSORSMEDIATOR_H
#define SENSORSMEDIATOR_H

#include "../Interfaces/ISensorsData.h"


/**
 * @brief Class used to exchange readings values between
 * sensors and other classess.
 * Basically this is class with getters and setters for sensor values
 * and does nothing more than that.
 * Remember that only one sensor should update each measurement!
 */
class SensorsMediator : public Interfaces::ISensorsData
{
public:
    SensorsMediator() {};

private:
    vector3Float accReading;
    vector3Float gyroReading;
    vector3Float magReading;
    float pressureReading;
    double latitude;
    double longitude;
    float bottomRangefinderReading;
    float temperatureReading;


public:
    void updateAcc(const vector3Float& newReading)
    {
        accReading = newReading;
    }

    void updateGyro(const vector3Float& newReading)
    {
        gyroReading = newReading;
    }

    void updateMag(const vector3Float& newReading)
    {
        magReading = newReading;
    }
    
    void updatePressure(float newReading)
    {
        pressureReading = newReading;
    }

    void updateLatLong(double latitude, double longitude)
    {
        this->latitude = latitude;
        this->longitude = longitude;
    }

    void updateBottomRangefinder(float newReading)
    {
        bottomRangefinderReading = newReading;
    }

    void updateTemperatureReading(float newReading)
    {
        temperatureReading = newReading;
    }


    // //// \/ \/
    // Interface
    // //// \/ \/

    vector3Float getAcc_normVector() override
    {
        return accReading;
    }
    
    vector3Float getGyro_degPerSec() override
    {
        return gyroReading;
    }

    vector3Float getMag_normVector() override
    {
        return magReading;
    }

    float getPressure_mbar() override
    {
        return pressureReading;
    }

    double getLatitude_deg() override
    {
        return latitude;
    }

    double getLongitude_deg() override
    {
        return longitude;
    }

    float getBottomRangefinderDistance_m() override
    {
        return bottomRangefinderReading;
    }

    float getTemperature_degC() override
    {
        return temperatureReading;
    }
};


#endif
