/**
 * @file INS.cpp
 * @author Jan Wielgus
 * @date 2021-08-26
 */

#include "INS.h"
#include "Instances/SensorInstances.h"
#include "Common/Utils.h"
#include "Common/Constants.h"
#include "config.h"

static FusionVector3 vector3FloatToFusion(const Common::vector3Float& vector3Float);


INS::INS()
{
    FusionBiasInitialise(&fusionBias, 0.5f, Config::MainInterval_s);
    FusionAhrsInitialise(&fusionAhrs, 0.5f); // TODO: test different settings
}


void INS::execute()
{
    // order is important
    updateAHRS();
    udpateAltitude();
    updateLatLong();
}


bool INS::resetAltitude()
{
    if (Instance::baro.isOperating())
    {
        refPressure = Instance::baro.getPressure_hPa();
        return true;
    }

    return false;
}


void INS::updateAHRS()
{
    FusionVector3 acc = vector3FloatToFusion(Instance::acc.getAcc_norm());
    FusionVector3 gyro = vector3FloatToFusion(Instance::gyro.getGyro_dps());

    // Update gyroscope bias correction algorithm
    gyro = FusionBiasUpdate(&fusionBias, gyro);

    // Update AHRS algorithm
    if (Instance::magn.isOperating())
    {
        FusionVector3 magn = vector3FloatToFusion(Instance::magn.getMagn_norm());
        FusionAhrsUpdate(&fusionAhrs, gyro, acc, magn, Config::MainInterval_s);
    }
    else
    {
        FusionAhrsUpdateWithoutMagnetometer(&fusionAhrs, gyro, acc, Config::MainInterval_s);
    }

    // Update quaternions
    FusionQuaternion fusionQuaternion = FusionAhrsGetQuaternion(&fusionAhrs);
    quaternion.r = fusionQuaternion.element.w;
    quaternion.i = fusionQuaternion.element.x;
    quaternion.j = fusionQuaternion.element.y;
    quaternion.k = fusionQuaternion.element.z;

    // Update Euler angles
    FusionEulerAngles eulerAngles = FusionQuaternionToEulerAngles(fusionQuaternion);
    angles_deg.x = eulerAngles.angle.pitch;
    angles_deg.y = eulerAngles.angle.roll;
    angles_deg.z = eulerAngles.angle.yaw;

    // Update Earth acceleration
    FusionVector3 fusionEarthAcc = FusionAhrsGetEarthAcceleration(&fusionAhrs);
    earthAcceleration_mps2.x = fusionEarthAcc.axis.x * Common::Consts::GravitationalAcceleration;
    earthAcceleration_mps2.y = fusionEarthAcc.axis.y * Common::Consts::GravitationalAcceleration;
    earthAcceleration_mps2.z = fusionEarthAcc.axis.z * Common::Consts::GravitationalAcceleration;
}


void INS::udpateAltitude()
{
    float curPressure = Instance::baro.getPressure_hPa();
    float curTemperature = Instance::temperature.getTemperature_degC();

    // altitude_m = Common::Utils::calculateAltitude(refPressure, curPressure, curTemperature);


    // Kalman Filter accelerometer based altitude
    float altitudeBaro_m = Common::Utils::calculateAltitude(refPressure, curPressure, curTemperature);

    float acceleration = getEarthAcceleration_mps2().z;
    float accelerationWithDecreasedPrecision = ((int16_t)(acceleration * 10)) / 10; // TODO: change to setting 0 if in some range, eg. <0.1, -0.1>
    altitude_m = kalman.update(altitudeBaro_m, accelerationWithDecreasedPrecision);

    // Serial1.print(5 * altitudeBaro_m);
    // Serial1.print('\t');
    // Serial1.println(5 * altitude_m);

    // if (cnt >= 1)
    // {
    //     if (cnt == 1)
    //     {
    //         resetAltitude();
    //         kalman.reset();
    //     }
    //     cnt--;
    // }



// // temp altitude calculation: (instead of Kalman)
//     float baroAlt = Common::Utils::calculateAltitude(refPressure, curPressure, curTemperature);
//     float baroVel = (baroAlt - lastBaroAlt) / Config::MainInterval_s; // derivative
//     lastBaroAlt = baroAlt;

//     verticalVelocity_mps += earthAcceleration_mps2.z * Config::MainInterval_s;
//     altitude_m += verticalVelocity_mps * Config::MainInterval_s;

//     float oneMinusBeta = 1.f - complementaryBeta;
//     verticalVelocity_mps = verticalVelocity_mps * complementaryBeta + baroVel * oneMinusBeta;
//     altitude_m = altitude_m * complementaryBeta + baroAlt * oneMinusBeta;
}


void INS::updateLatLong()
{
    // TODO: update latitude and longitude
}




inline FusionVector3 vector3FloatToFusion(const Common::vector3Float& vector3Float)
{
    return {
        vector3Float.x,
        vector3Float.y,
        vector3Float.z
    };
}
