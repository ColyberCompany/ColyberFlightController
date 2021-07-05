#ifndef TINYGPSADAPTER_H
#define TINYGPSADAPTER_H

#include "Sensor.h"
#include "SensorsMediator.h"
#include <IExecutable.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>


class TinyGPSAdapter: public Sensor, public IExecutable
{
    TinyGPSPlus gps;

public:
    TinyGPSAdapter(SensorsMediator& SensorsMediator);

    void execute() override;

    bool initialize() override;
    const char* getName() override
    {
        return "GPS";
    }
};



#endif