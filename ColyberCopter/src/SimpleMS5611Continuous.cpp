/**
 * @file SimpleMS5611Continuous.cpp
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @date 2021-02-27
 * 
 */

#include "../Sensors/SimpleMS5611Continuous.h"


SimpleMS5611Continuous::MS5611ReadingTask::MS5611ReadingTask(SimpleMS5611Continuous& _ms5611)
    : ms5611(_ms5611)
{
}


void SimpleMS5611Continuous::MS5611ReadingTask::execute()
{
    if (shouldGetPressureAndRequestPressure())
    {
        ms5611.getRawPressureFromDevice();
        ms5611.requestPressureFromDevice();
    }
    else if (shouldGetPressureAndRequestTemperature())
    {
        ms5611.getRawPressureFromDevice();
        ms5611.requestTemperatureFromDevice();
    }
    else if (shouldGetTemperatureAndRequestPressure())
    {
        ms5611.getRawTemperatureFromDevice();
        ms5611.requestPressureFromDevice();
        
        actionCounter = 0; // 0 because incrementation is at the end
    }
    else // first pressure request, if actionCounter is 0 at the beginning
    {
        // Should run this only once
        ms5611.requestPressureFromDevice();
    }


    ms5611.calculatePressureAndTemperatureFromRawData();
    ms5611.averagePressure();
    ms5611.updateSmoothPressure();
    ms5611.executeNewReadingEvent();

    actionCounter++;
}


bool SimpleMS5611Continuous::MS5611ReadingTask::shouldGetPressureAndRequestPressure()
{
    return actionCounter >= 1 &&
        actionCounter < SimpleMS5611Continuous::PressurePerTemperatureRequests;
}

bool SimpleMS5611Continuous::MS5611ReadingTask::shouldGetPressureAndRequestTemperature()
{
    return actionCounter == SimpleMS5611Continuous::PressurePerTemperatureRequests;
}

bool SimpleMS5611Continuous::MS5611ReadingTask::shouldGetTemperatureAndRequestPressure()
{
    return actionCounter == (SimpleMS5611Continuous::PressurePerTemperatureRequests + 1);
}







SimpleMS5611Continuous::SimpleMS5611Continuous(ITasker& _tasker)
    : tasker(_tasker),
    pressureFilter_pascal(PressurePerTemperatureRequests + 1),
    readingTask(*this)
{
}


bool SimpleMS5611Continuous::initialize()
{
    if (!SimpleMS5611::initialize())
        return false;

    static bool initialized = false;
    if (!initialized)
    {
        float readingTaskFrequency = 1000000.f / RequestWaitTime_us;
        tasker.addTask(&readingTask, readingTaskFrequency);
        initialized = true;
    }
    
    return true;
}


float SimpleMS5611Continuous::getPressure() 
{
    return pressure_mbar;
}


float SimpleMS5611Continuous::getSmoothPressure()
{
    return smoothPressure_mbar;
}


void SimpleMS5611Continuous::setNewReadingEvent(IExecutable* newReadingEvent)
{
    this->newReadingEvent = newReadingEvent;
}


void SimpleMS5611Continuous::averagePressure()
{
    // Pressure is the average value of PressurePerTemperatureRequests + 1 readings
    pressureFilter_pascal.update(pressure_pascal);
    pressure_mbar = pressureFilter_pascal.getFilteredValueFloat() / 100.f;
}


void SimpleMS5611Continuous::updateSmoothPressure()
{
    if (abs(smoothPressure_mbar - pressure_mbar) > 1)
		smoothPressure_mbar = smoothPressure_mbar*0.72f + pressure_mbar*0.28f;
	else
		smoothPressure_mbar = smoothPressure_mbar*0.96f + pressure_mbar*0.04f;
}


void SimpleMS5611Continuous::executeNewReadingEvent()
{
    if (newReadingEvent != nullptr)
        newReadingEvent->execute();
}
