#include "AppSystem.h"
#include "BSP_LED.h"
#include "BSP_Delay.h"
#include "Scheduler.h"

static void Task_LED(void)
{
    BSP_LED_Toggle(BSP_LED_COLOR_BLUE);
    BSP_LED_Toggle(BSP_LED_COLOR_RED);
}

static Task_t led_task =
{
    .name="LED Task",

    .function=Task_LED,

    .period_ms=500,

    .last_run=0
};


void AppSystem_Init()
{

    BSP_Delay_Init();

    BSP_LED_Init();
    Scheduler_Init();

    Scheduler_Register(
        &led_task
    );
}

void AppSystem_Main()
{
    Scheduler_Run();
}

