/**
 * @file LogType.h
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @date 2021-04-09
 * 
 */

#ifndef LOGTYPE_H
#define LOGTYPE_H


namespace Enums
{
    enum LogType
    {
        Base = 1,
        Info = 2,
        Debug = 4,
        Error = 8,

        Count = 4
    };
}


#endif
