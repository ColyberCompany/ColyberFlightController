/**
 * @file ISensorsData.h
 * @author Jan Wielgus
 * @date 2020-07-30
 * 
 */

#ifndef ISENSORSDATA_H
#define ISENSORSDATA_H

#include "../Common/Vector3.h"


namespace Interfaces
{
    class ISensorsData
    {
    public:
        virtual ~ISensorsData() {}

        virtual vector3Float getAcc_normVector() = 0; // TODO: think whether to return by value or by (const) reference
        virtual vector3Float getGyro_degPerSec() = 0;
        virtual vector3Float getMag_normVector() = 0;
        virtual float getPressure_mbar() = 0;
        virtual double getLatitude_deg() = 0;
        virtual double getLongitude_deg() = 0;
        virtual float getBottomRangefinderDistance_m() = 0;
        virtual float getTemperature_degC() = 0;
    };
}


#endif
