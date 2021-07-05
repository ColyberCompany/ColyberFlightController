#include "../Sensors/TinyGPSAdapter.h"
#include "../Enums/BaudRateTypes.h"

TinyGPSAdapter::TinyGPSAdapter(SensorsMediator& sensorsMediator)
    : Sensor(Enums::SensorTypes::GPS, sensorsMediator)
{
}

bool TinyGPSAdapter::initialize()
{
    Serial2.begin(Enums::BAUD_9600);

    if(gps.charsProcessed() < 10 && gps.encode(Serial.read()) == 0) 
        return false;
    
    return true;
}

void TinyGPSAdapter::execute()
{
    while(Serial2.available() > 0)
    {
        if(gps.encode(Serial2.read()))
        {
            if(gps.location.isValid())
            {
                sensorsMediator.updateLatLong(gps.location.lat(), gps.location.lng());
            }    
        }      
    }   
}