#include "BSP_Time.h"
#include "stm32f1xx_hal.h"


uint32_t BSP_GetTick(void)
{
    return HAL_GetTick();
}