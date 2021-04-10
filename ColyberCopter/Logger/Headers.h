/**
 * @file Headers.h
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @date 2021-04-10
 * 
 */

#ifndef HEADERS_H
#define HEADERS_H

#include "../Enums/LogType.h"


const char Headers[] = {'B', 'I', 'D', 'E'}; 


static_assert(Enums::LogType::Count == (sizeof(Headers) / sizeof(Headers[0])), "Every LogType value must have proper header in Headers table");


#endif
