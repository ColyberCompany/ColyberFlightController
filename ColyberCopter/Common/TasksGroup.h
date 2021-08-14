/**
 * @file TasksGroup.h
 * @author Antoni Wielgus
 * @brief This classes is used to group tasks into one
 * @date 2021-08-09
 */

#ifndef TASKSGROUP_H
#define TASKSGROUP_H

#include <IExecutable.h>

#ifdef ARDUINO
    #include <Arduino.h>
#endif


namespace Common
{
    class TasksGroup : public IExecutable
    {
        IExecutable** tasksArray;
        const uint8_t MaxTasksAmount;
        uint8_t tasksAmount = 0;


    public:
        TasksGroup(uint8_t maxTasksAmount);
        ~TasksGroup();

        bool addTask(IExecutable* task);
        void execute() override;
    };
}


#endif