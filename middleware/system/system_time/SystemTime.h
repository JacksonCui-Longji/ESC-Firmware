#ifndef SYSTEM_TIME_H
#define SYSTEM_TIME_H

#include "esc_types.h"

extern void SystemTime_Init(void);
extern uint32_t SystemTime_GetMs(void);
extern uint32_t SystemTime_GetElapsedMs(void);


#endif