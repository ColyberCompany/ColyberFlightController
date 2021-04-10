/**
 * @file Logger.h
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @date 2021-04-09
 * 
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "Headers.h"
#include "../Common/Pair.h"
#include "../Enums/LogType.h"
#include "../config.h"
#include <DataBuffer.h>
#include <GrowingArray.h>
#include <ITransmitter.h>


class Logger
{
    typedef Pair<Enums::LogType, ITransmitter*> Binding;

    GrowingArray<Binding> bindings;

    char buffer[Config::MaxLogSize + Enums::LogType::Count + 1];
    uint8_t startIndex;
    uint8_t endIndex = Enums::LogType::Count + 1;

    void addToBuffer(const char* str);
    void addToBuffer(int number);

    void prepareHeader(Enums::LogType logType);

    template <class T>
    void log(Enums::LogType logType, T item);

    template <class First, class... Args>
    void log(Enums::LogType logType, First first, Args... args);

public:
    void bind(Enums::LogType logType, ITransmitter* transmitter);
    void unbind(Enums::LogType logType, ITransmitter* transmitter);
    GrowingArray<ITransmitter*> getTransmitters(Enums::LogType logType);

    template <class First, class... Args>
    void error(First first, Args... args);

    template <class First, class... Args>
    void debug(First first, Args... args);

    template <class First, class... Args>
    void info(First first, Args... args);
};


template <class T>
void Logger::log(Enums::LogType logType, T item)
{
    addToBuffer(item);

    auto iter = bindings.getIterator();
    while (iter->hasNext())
    {
        auto binding = iter->next();
        if (binding.first & logType)
        {
            prepareHeader((Enums::LogType)(logType & binding.first));
            binding.second->send((uint8_t*)buffer[startIndex], endIndex - startIndex);
        }
    }

    endIndex = Enums::LogType::Count + 1;
}


template<class First, class... Args>
void Logger::log(Enums::LogType logType, First first, Args... args)
{
    addToBuffer(first);
    log(logType, args...);
}


template <class First, class... Args>
void Logger::error(First first, Args... args)
{
    log(Enums::LogType::Error | Enums::LogType::Debug | Enums::LogType::Info | Enums::LogType::Base, first, args...);
}


template <class First, class... Args>
void Logger::debug(First first, Args... args)
{
    log(Enums::LogType::Debug | Enums::LogType::Base, first, args...);
}


template <class First, class... Args>
void Logger::info(First first, Args... args)
{
    log(Enums::LogType::Info | Enums::LogType::Base, first, args...);
}


#endif
