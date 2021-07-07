/**
 * @file FloatAxisVector.h
 * @author Jan Wielgus
 * @brief This class is used to create one, two or three
 * dimensional vector to store different sized vector
 * in a unified way.
 * @date 2020-11-25
 * 
 */

#ifndef FLOATAXISVECTOR_H
#define FLOATAXISVECTOR_H

#ifdef ARDUINO
    #include <Arduino.h>
#endif

#include "../Enums/AxisType.h"


class FloatAxisVector
{
private:
    float* valuesArray;
    uint8_t axesAmt; // at most 3

public:
    /**
     * @brief Creates zero axis object. 
     */
    FloatAxisVector()
    {
        allocateMemory(0);
    }

    /**
     * @brief Creates one axis object.
     */
    FloatAxisVector(float x)
    {
        allocateMemory(1);
        valuesArray[0] = x;
    }

    /**
     * @brief Creates two axes object.
     */
    FloatAxisVector(float x, float y)
    {
        allocateMemory(2);
        valuesArray[0] = x;
        valuesArray[1] = y;
    }

    /**
     * @brief Creates three axes object.
     */
    FloatAxisVector(float x, float y, float z)
    {
        allocateMemory(3);
        valuesArray[0] = x;
        valuesArray[1] = y;
        valuesArray[2] = z;
    }

    /**
     * @brief Copy ctor.
     * @param other Other vector to make a copy.
     */
    FloatAxisVector(const FloatAxisVector& other)
    {
        axesAmt = other.axesAmt;
        valuesArray = new float[axesAmt];
        for (uint8_t i = 0; i < axesAmt; i++)
            valuesArray[i] = other.valuesArray[i];
    }

    FloatAxisVector(FloatAxisVector&& toMove)
    {
        axesAmt = toMove.axesAmt;
        valuesArray = toMove.valuesArray;

        toMove.axesAmt = 0;
        toMove.valuesArray = nullptr;
    }

    ~FloatAxisVector()
    {
        delete[] valuesArray;
    }

    /**
     * @brief Assignment operator. Changes amount of axes and their values.
     * @param other Original axis vector to copy.
     */
    FloatAxisVector& operator=(const FloatAxisVector& other)
    {
        if (this != &other)
        {
            if (axesAmt != other.axesAmt)
            {
                delete[] valuesArray;
                axesAmt = other.axesAmt;
                valuesArray = new float[axesAmt];
            }

            for (int i = 0; i < axesAmt; i++)
                    valuesArray[i] = other.valuesArray[i];
        }

        return *this;
    }

    FloatAxisVector& operator=(FloatAxisVector&& toMove)
    {
        if (this != &toMove)
        {
            delete[] valuesArray;
            axesAmt = toMove.axesAmt;
            valuesArray = toMove.valuesArray;

            toMove.axesAmt = 0;
            toMove.valuesArray = nullptr;
        }

        return *this;
    }

    /**
     * @param axis Axis to get value from
     * (for one dimensional types put AxisX)
     * @return Value on specified axis.
     */
    float getAxis(Enums::AxisType axis)
    {
        if ((uint8_t)axis < axesAmt)
            return valuesArray[(uint8_t)axis];
        return 0;
    }

    /**
     * @brief Setter of one axis value. 
     * @param value New value for the axis.
     * @param axis Which axis.
     */
    void setAxis(float value, Enums::AxisType axis)
    {
        if ((uint8_t)axis < axesAmt)
            valuesArray[(uint8_t)axis] = value;
    }

    /**
     * @brief Axes amount getter.
     */
    uint8_t getAxesAmt()
    {
        return axesAmt;
    }

private:
    void allocateMemory(uint8_t axesAmt)
    {
        if (axesAmt > 3)
            axesAmt = 3;

        this->axesAmt = axesAmt;
        valuesArray = new float[axesAmt];
    }
};


#endif
