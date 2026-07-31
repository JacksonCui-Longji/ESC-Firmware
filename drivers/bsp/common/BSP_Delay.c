#include "BSP_Delay.h"

#include "stm32f1xx_hal.h"


void BSP_Delay_Init(void)
{

}


void BSP_Delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}