/**
 * @file SimpleMS5611Continuous.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief Simple MS5611 barometer library that perform continuous
 * measurements using SimpleTasker library.
 * @date 2021-02-27
 * 
 */


#ifndef SIMPLEMS5611CONTINUOUS_H
#define SIMPLEMS5611CONTINUOUS_H

#include <SimpleMS5611.h>
#include <ITasker.h>
#include <IExecutable.h>
#include <AverageFilter.h>


class SimpleMS5611Continuous : public SimpleMS5611
{
    class MS5611ReadingTask : public Task
    {
        SimpleMS5611Continuous& ms5611;
        uint8_t actionCounter = 0; // used to get temperature every 20th measurement, from 1 to PressurePerTemperatureRequests, 0 indicates first pressure request

    public:
        MS5611ReadingTask(SimpleMS5611Continuous& ms5611);

        /**
         * @brief Reads pressure and reads temperature every 20th execution.
         * 
         * @return  
         */
        void execute() override;

    
    private:
        bool shouldGetPressureAndRequestPressure();
        bool shouldGetPressureAndRequestTemperature();
        bool shouldGetTemperatureAndRequestPressure();
    };


    ITasker& tasker;
    AverageFilter<int32_t> pressureFilter_pascal;

    float smoothPressure_mbar = 0.f;
    MS5611ReadingTask readingTask; // Instance of tasker task that performs reading pressure and temperature
    IExecutable* newReadingEvent = nullptr;

    static const uint8_t PressurePerTemperatureRequests = 19; // How many pressure requests are per one temperature request


public:
    SimpleMS5611Continuous(ITasker& tasker);

    /**
     * @brief Initialize the MS5611 baro.
	 * Need to be called before the first use.
     * Adds one task to the tasker.
	 * @return false if something went wrong,
	 * true otherwise. 
     */
    bool initialize() override;

    /**
     * @brief Getter of the newest continuously read pressure value.
     * @return Newest pressure value [mbar].
     */
    float getPressure() override;

    /**
     * @brief Getter of the newest continuously read
     * smoothed pressure value with a little bit more lag.
     * @return Newest smoothed pressure value [mbar].
     */
    float getSmoothPressure();

    /**
     * @brief Set event called when new reading will arrive.
     * @param newReadingEvent Pointer to the executable.
     */
    void setNewReadingEvent(IExecutable* newReadingEvent);


private:
    void averagePressure();
    void updateSmoothPressure();
    void executeNewReadingEvent();


    friend class MS5611ReadingTask;
};


#endif
