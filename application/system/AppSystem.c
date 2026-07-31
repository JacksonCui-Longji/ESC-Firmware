#include "AppSystem.h"
#include "BSP_LED.h"
#include "BSP_Delay.h"


void AppSystem_Init()
{

    BSP_Delay_Init();

    BSP_LED_Init();
}

void AppSystem_Main()
{
    BSP_LED_Toggle(BSP_LED_COLOR_BLUE);
    BSP_Delay_ms(500);
}

