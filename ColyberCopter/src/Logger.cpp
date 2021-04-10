/**
 * @file Logger.cpp
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @date 2021-04-10
 * 
 */

#include "../Logger/Logger.h"
#include "../Enums/LogType.h"
#include "../Logger/Headers.h"

using Enums::LogType;


void Logger::addToBuffer(const char* str)
{
    while(*str != '\0' && endIndex < Config::MaxLogSize + Enums::LogType::Count + 1)
    {
        buffer[endIndex++] = *(str++);
    }
}


void Logger::addToBuffer(int number)
{
    int n = 0;
    for (int i = 1; number / i; i *= 10)
        n++;

    if (endIndex + n - 1 < Config::MaxLogSize + Enums::LogType::Count + 1)
    {
        int j = 1;
        for (int i = 0; i < n; i++)
        {
            buffer[endIndex + n - 1 - i] = ((number / j) % 10) + '0';
            j *= 10;
        }
    }
    else
    {
        while(endIndex < Config::MaxLogSize + Enums::LogType::Count + 1)
        {
            buffer[endIndex++] = '*';
        }
    }
}


inline void Logger::prepareHeader(LogType logType)
{
    startIndex = LogType::Count;

    LogType type = LogType::Base;
    for (uint8_t i = 0; i < Enums::LogType::Count; i++)
    {
        if (logType & type)
            buffer[--startIndex] = Headers[i];

        type = (LogType)(type*2);
    }
}


void Logger::bind(LogType logType, ITransmitter* transmitter)
{
    if (transmitter == nullptr)
        return;

    auto iter = bindings.getIterator();
    bool transmitterExists = false;
    while (iter->hasNext())
    {
        auto binding = iter->next();
        if (binding.second == transmitter)
        {
            binding.first = (LogType)(logType | binding.first);
            transmitterExists = true;
        }
    }

    if (!transmitterExists)
        bindings.add(Binding(logType, transmitter));
}


void Logger::unbind(LogType logType, ITransmitter* transmitter)
{
    if (transmitter == nullptr)
        return;

    auto iter = bindings.getIterator();
    while (iter->hasNext())
    {
        auto binding = iter->next();
        if (binding.second == transmitter)
            binding.first = (LogType)(~logType & binding.first);
    }
}


GrowingArray<ITransmitter*> Logger::getTransmitters(LogType logType)
{
    auto iter = bindings.getIterator();
    uint8_t n = 0;
    while (iter->hasNext())
        if (iter->next().first == logType) n++;

    auto array = GrowingArray<ITransmitter*>(n);

    iter = bindings.getIterator();
    while (iter->hasNext())
        array.add(iter->next().second);

    return array;
}
