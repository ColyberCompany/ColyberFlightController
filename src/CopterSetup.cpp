/**
 * @file CopterSetup.cpp
 * @author Jan Wielgus
 * @brief In this file all classes are connected together.
 * @date 2020-11-27
 * 
 */

// General:
#include "CopterSetup.h"
#include "config.h"
#include <Tasker.h>
#include "Tasks.h"
#include "Common/Constants.h"
#include "Common/Utils.h"
// Failsafe:
#include "Failsafe/FailsafeManager.h"
#include "Failsafe/FailsafeActions/DisarmMotors.h"
#include "Failsafe/FailsafeScenarios/CommunicationLost.h"
#include "Failsafe/FailsafeScenarios/TiltExceeding.h"
// Flight modes:
#include "FlightModes/UnarmedFlightMode.h"
#include "FlightModes/StabilizeFlightMode.h"
#include "FlightModes/AltHoldFlightMode.h"
#include "NavigationSystem/VirtualPilot.h"
// Navigation system:
#include "NavigationSystem/INS.h"
// Motors:
#include "Motors/Motors.h"
#include "Motors/QuadXMotors.h"
#include "Motors/NoMotors.h"
// Communication:
#include <StreamComm.h>
#include <PacketCommunication.h>
#include "Debug/SerialDebugMessenger.h"
#include "Communication/CommData.h"
#include "Communication/DataPackets.h"
#include "Common/TasksGroup.h"
// Sensors base:
#include "Sensors/Base/Accelerometer.h"
#include "Sensors/Base/Gyroscope.h"
#include "Sensors/Base/Magnetometer.h"
#include "Sensors/Base/Barometer.h"
#include "Sensors/Base/GPS.h"
#include "Sensors/Base/Rangefinder.h"
#include "Sensors/Base/TemperatureSensor.h"
#include "Sensors/NoSensor.h"
// Sensors:
#include "Sensors/SimpleMPU6050Handler.h"
#include "Sensors/SimpleHMC5883LHandler.h"
#include "Sensors/SimpleMS5611Handler.h"

#include "KalmanFilter.h"

using namespace Interfaces;


// Setup functions
void addTasksToTasker();
void setupFailsafe();
void initializeSensors();
void setupFlightModes();
void setupCommunication();

// Helper functions
bool initSensor(Sensor* sensorToInit);


// https://github.com/stm32duino/wiki/wiki/API#hardwareserial
//HardwareSerial Serial1(PA10, PA9); // Serial1 is compiling, but I don't know on which pins
HardwareSerial Serial2(PA3, PA2);
//HardwareSerial Serial3(PB11, PB10);


namespace Assemble
{
    Tasker tasker(Config::MaxTaskerTasks);
    SerialDebugMessenger serialDebugMessenger(Serial1);

    namespace Motors {
        QuadXMotors quadXMotors;
        NoMotors noMotors;
    }

    namespace NavigationSystem {
        INS ins;
    }

    namespace Communication {
        PacketComm::StreamComm<Config::RmtCtrlMaxComBufferSize> rmtCtrlCommStream(&Serial2);
        PacketComm::PacketCommunication rmtPacketComm(&rmtCtrlCommStream); // Remote comm instance
    }

    namespace FlightModes {
        UnarmedFlightMode unarmedFlightMode;
        StabilizeFlightMode stabilizeFlightMode;
        AltHoldFlightMode altHoldFlightMode(stabilizeFlightMode);
    }

    VirtualPilot virtualPilotInstance(FlightModes::unarmedFlightMode);

    namespace Sensors {
        SimpleMPU6050Handler simpleMPU6050Handler;
        SimpleHMC5883LHandler simpleHMC5883LHandler;
        SimpleMS5611Handler simpleMS5611Handler;
        // other sensors..
        NoSensor noSensor;
    }

    namespace Failsafe { // TODO: try to improve names of objects inside
        FailsafeManager failsafeManager;
        FailsafeActions::DisarmMotors failsafeActionDisarmMotors;
        FailsafeScenarios::CommunicationLost failsafeScenarioCommLost(&failsafeActionDisarmMotors);
        FailsafeScenarios::TiltExceeding failsafeTiltExceeding(&failsafeActionDisarmMotors);
    }

    namespace TaskGroups {
        Common::TasksGroup mainFrequency(5);
        Common::TasksGroup oneHertz(4);
    }
}


namespace Instance
{
// MainInstances:
    Tasker& tasker = Assemble::tasker;
    INS& ins = Assemble::NavigationSystem::ins;
    IVirtualPilot& virtualPilot = Assemble::virtualPilotInstance;
    PacketComm::PacketCommunication& pilotPacketComm = Assemble::Communication::rmtPacketComm;
    FailsafeManager& failsafeManager = Assemble::Failsafe::failsafeManager;
    DebugMessenger& debMes = Assemble::serialDebugMessenger;


// SensorInstances:
    using Assemble::Sensors::noSensor;
    Accelerometer& acc = Assemble::Sensors::simpleMPU6050Handler;
    Gyroscope& gyro = Assemble::Sensors::simpleMPU6050Handler;
    Magnetometer& magn = Assemble::Sensors::simpleHMC5883LHandler;
    Barometer& baro = Assemble::Sensors::simpleMS5611Handler;
    TemperatureSensor& temperature = Assemble::Sensors::simpleMS5611Handler;

    // Sensor& gps = noSensor;
    // Sensor& btmRangefinder = noSensor;

// MotorsInstance:
    Motors& motors = Assemble::Motors::quadXMotors;
}


class : public IExecutable
{
    
    void execute() override {
        using Common::Utils::printVector3;

        //printVector3(Instance::ins.getAngles_deg());
    }

} debugTask;




void setupDrone()
{
    using Instance::debMes;
    using Common::Consts::OKText;

    debMes.enableAndInitialize(); // Comment this line to disable all debug messages
    debMes.showMessage("Beginning drone setup");
    delay(200);


    debMes.showMessage("Failsafe");
    setupFailsafe();
    debMes.showMessage(OKText);


    debMes.showMessage("Sensors");
    initializeSensors();
    debMes.showMessage(OKText);


    debMes.showMessage("FlightModes");
    setupFlightModes();
    debMes.showMessage(OKText);


    debMes.showMessage("RmtCtrlComm");
    setupCommunication();
    debMes.showMessage(OKText);


    debMes.showMessage("Motors");
    Instance::motors.initializeMotors(); // TODO: initialize in separate func and check return value
    debMes.showMessage(OKText);


    addTasksToTasker();
    

    pinMode(LED_BUILTIN, OUTPUT); // TODO: this is temporary, figure something out

    debMes.showMessage("Drone setup is complete!");
}



void setupFailsafe()
{
    Instance::failsafeManager.initializeFailsafe();
    Instance::failsafeManager.addFailsafeScenario(&Assemble::Failsafe::failsafeScenarioCommLost);
    Instance::failsafeManager.addFailsafeScenario(&Assemble::Failsafe::failsafeTiltExceeding);
}


void initializeSensors()
{
    Wire.begin();
    delay(100);


    // TODO: make a list from sensors and add enum with sensor types
    initSensor(&Instance::acc);
    initSensor(&Instance::gyro);
    //initSensor(&Instance::magn); // TODO: calibrate magnetometer and initialize it
    initSensor(&Instance::baro);
    //initSensor(&Instance::gps);
    //initSensor(&Instance::btmRangefinder);
    // new sensors goes here...
    

    // Set fast 400kHz I2C clock
    Wire.setClock(400000L);
}


void setupFlightModes()
{
    Instance::virtualPilot.addFlightMode(&Assemble::FlightModes::unarmedFlightMode);
    Instance::virtualPilot.addFlightMode(&Assemble::FlightModes::stabilizeFlightMode); // TODO: think whether to pass flight modes by reference
    Instance::virtualPilot.addFlightMode(&Assemble::FlightModes::altHoldFlightMode);
    // add other flight modes...

    Instance::virtualPilot.initializeFlightModes(); // TODO: this method returns false if not all flight modes were initialized. This should be checked!
}


void addTasksToTasker()
{
    using Instance::tasker;

    Assemble::TaskGroups::mainFrequency.addTask(&Assemble::Sensors::simpleMPU6050Handler);
    Assemble::TaskGroups::mainFrequency.addTask(&Assemble::NavigationSystem::ins);
    Assemble::TaskGroups::mainFrequency.addTask(&Assemble::virtualPilotInstance);
    Assemble::TaskGroups::mainFrequency.addTask(&Assemble::Motors::quadXMotors);
    tasker.addTask_us(&Assemble::TaskGroups::mainFrequency, Config::MainInterval_us);

    Assemble::TaskGroups::oneHertz.addTask(&Tasks::builtinDiodeBlink);
    tasker.addTask_Hz(&Assemble::TaskGroups::oneHertz, 1.f);

    tasker.addTask_Hz(&Assemble::Failsafe::failsafeManager, 10);
    tasker.addTask_Hz(&Assemble::Sensors::simpleHMC5883LHandler, 75);
    tasker.addTask_us(&Assemble::Sensors::simpleMS5611Handler, SimpleMS5611Handler::RequestWaitTime_us, TaskType::NO_CATCHING_UP);
    tasker.addTask_Hz(&Tasks::rmtCtrlReceiving, Config::RmtCtrlReceivingFrequency_Hz);
    tasker.addTask_Hz(&Tasks::rmtCtrlSendingDroneData, 10);
    tasker.addTask_Hz(&debugTask, 50);
}


void setupCommunication()
{
    Serial2.begin(Config::RmtCtrlSerialBaudRate);
    Instance::pilotPacketComm.adaptConnStabilityToFrequency(Config::RmtCtrlReceivingFrequency_Hz);

    Instance::pilotPacketComm.registerReceivePacket(&DataPackets::steering);
    Instance::pilotPacketComm.registerReceivePacket(&DataPackets::flightModeChange);
    Instance::pilotPacketComm.registerReceivePacket(&DataPackets::pidTuning);
    // add other data packets that could be received...
}






bool initSensor(Sensor* sensorToInit)
{
    Instance::debMes.showMessage("Initializing:");
    Instance::debMes.showMessage(sensorToInit->getName());

    bool sensorInitResult = sensorToInit->initialize();
    delay(20);

    if (sensorInitResult == false)
    {
        Instance::debMes.showMessage("failed");
        Instance::debMes.showError(478792);
    }
    else
        Instance::debMes.showMessage("success");

    return sensorInitResult;
}
