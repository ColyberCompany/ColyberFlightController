/**
 * @file MS5611Adapter.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief Adapter for MS5611 barometer.
 * @date 2021-02-25
 * 
 */

#ifndef MS5611ADAPTER_H
#define MS5611ADAPTER_H

#include "Sensor.h"
#include "SensorsMediator.h"
#include "SimpleMS5611Continuous.h"
#include <ITasker.h>
#include <IExecutable.h>


/**
 * @brief Adapter class for MS5611 barometer library.
 * Pressure background measurement begins after initialize() method call.
 */
class MS5611Adapter : public Sensor, private IExecutable
{
    SimpleMS5611Continuous baro;


public:
    MS5611Adapter(SensorsMediator& sensorsMediator, ITasker& tasker);

    bool initialize() override;
    const char* getName() override;

private:
    void execute() override;
};


#endif
