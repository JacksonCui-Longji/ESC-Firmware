#include "SystemTime.h"
#include "BSP_Time.h"


static uint32_t s_lastTick = 0;
static uint32_t s_elapsedMs = 0;


void SystemTime_Init(void)
{
    s_lastTick = BSP_GetTick();
    s_elapsedMs = 0;
}


uint32_t SystemTime_GetMs(void)
{
    return BSP_GetTick();
}


uint32_t SystemTime_GetElapsedMs(void)
{
    uint32_t current;

    current = BSP_GetTick();
    s_elapsedMs = current - s_lastTick;
    s_lastTick = current;
    
    return s_elapsedMs;
}