/**
 * @file IRotationCalculation.h
 * @author Jan Wielgus
 * @date 2020-07-30
 * 
 */

#ifndef IROTATIONCALCULATION_H
#define IROTATIONCALCULATION_H

#include "../Common/Vector3.h"


namespace Interfaces
{
    class IRotationCalculation
    {
    public:
        virtual ~IRotationCalculation() {}

        virtual void updateRotationCalculation() = 0;
        virtual vector3Float getAngles_deg() = 0;
        virtual vector3Float getAngles_rad() = 0;
    };
}


#endif
