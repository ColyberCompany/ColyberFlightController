/**
 * @file config.h
 * @author Jan Wielgus
 * @brief This is the configuraiton file.
 * @date 2020-11-09
 * 
 */

#ifndef CONFIG_H
#define CONFIG_H

#ifdef ARDUINO
    #include <Arduino.h>
#endif

#include "Enums/BaudRateTypes.h"
#include "Common/Vector3.h"


namespace Config
{
    extern const uint16_t MainFrequency_Hz;
    extern const uint16_t MainInterval_us;
    extern const float MainInterval_s; // delta time between next main loop executions

    extern const Enums::BaudRateTypes RmtCtrlSerialBaudRate;

    extern const size_t RmtCtrlMaxComBufferSize;
    extern const size_t RmtCtrlMaxQueuedBuffers;
    extern const uint16_t RmtCtrlReceivingFrequency_Hz;

    extern const uint8_t MaxTaskerTasks;

    extern const float TiltExceedingAngleThreshold;

    extern const float AccLPFCutOffFreq;
    extern const float GyroLPFCutOffFreq;

    extern const uint16_t ThrottleStickCenter;

    extern const float LevelingPID_kP;
    extern const float LevelingPID_kI;
    extern const float LevelingPID_kD;
    extern const uint16_t LevelingPID_IMax;

    extern const float HeadHoldPID_kP;
    extern const float HeadHoldPID_kI;
    extern const float HeadHoldPID_kD;
    extern const uint16_t HeadHoldPID_IMax;

    extern const float AltHoldPID_kP;
    extern const float AltHoldPID_kI;
    extern const float AltHoldPID_kD;
    extern const uint16_t AltHoldPID_IMax;

    extern const vector3Int16 AccOffset;
    extern const vector3Int16 GyroOffset;
}


#endif

