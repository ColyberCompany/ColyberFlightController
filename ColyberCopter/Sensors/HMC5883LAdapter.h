/**
 * @file HMC5883LAdapter.h
 * @author Jan Wielgus
 * @brief Adapter for HMC5883L magnetometer.
 * @date 2020-11-26
 * 
 */

#ifndef HMC5883LADAPTER_H
#define HMC5883LADAPTER_H

#include "Sensor.h"
#include <SimpleHMC5883L.h>
#include <SimpleMPU6050.h>
#include "SensorsMediator.h"
#include <Task.h>
#include "../Common/Vector3.h"
#include "../Common/Counter.h"


class HMC5883LAdapter: public Sensor, public Task
{
    SimpleHMC5883L compass;
    SimpleMPU6050* mpu; // used to enable compass bypass on GY86.

    // calibration
    SimpleHMC5883L::vector3Int16 mins;
    SimpleHMC5883L::vector3Int16 maxs;
    Counter calibCounter;


public:
    HMC5883LAdapter(SensorsMediator& sensorsMediator, SimpleMPU6050* mpu6050);

    bool initialize() override;

    void execute() override;

    /**
     * @brief During calibration make every possible rotation with a magnetometer.
     * @param amtOfSamples Amount of new readings that will influence
     * the calibration process.
     * @return Time that calibration process will take [in seconds].
     */
    uint16_t startBackgroundCalibration(uint16_t amtOfSamples) override;

    FloatAxisVector getOffset() const override;

    void setOffset(FloatAxisVector offset) override;

    const char* getName() override
    {
        return "compass";
    }

private:
    void calibrationLoop();
};


#endif

