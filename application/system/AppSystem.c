#include "AppSystem.h"
#include "BSP_LED.h"
#include "BSP_Delay.h"
#include "Scheduler.h"
#include "Event.h"

static void Task_LED(void)
{
    if(EVENT_STATE_PENDING == Event_IsPending(EVENT_LED))
    {
        BSP_LED_State_Set(BSP_LED_COLOR_BLUE, BSP_LED_OFF);
        BSP_LED_State_Set(BSP_LED_COLOR_RED, BSP_LED_ON);
        BSP_LED_State_Set(BSP_LED_COLOR_GREEN, BSP_LED_OFF);
        Event_Clear(EVENT_LED);
    }
    else
    {
        BSP_LED_State_Set(BSP_LED_COLOR_BLUE, BSP_LED_ON);
        BSP_LED_State_Set(BSP_LED_COLOR_RED, BSP_LED_OFF);
        BSP_LED_State_Set(BSP_LED_COLOR_GREEN, BSP_LED_OFF);
    }
}

static Task_t led_task =
{
    .name = "LED Task",
    .function = Task_LED,
    .period_ms = 500,
    .last_run = 0
};

static void Task_AppSystemHeartbeat(void)
{
    Event_Post(EVENT_LED); // test code, further feed watchdog
}

static Task_t AppSystem_heartbeat_task = 
{
    .name = "AppSystem Heart Beat",
    .function = Task_AppSystemHeartbeat,
    .period_ms = 1000,
    .last_run = 0
};

void AppSystem_Init()
{
    // Middleware Init
    Event_Init();
    Scheduler_Init();

    // Driver Init
    BSP_Delay_Init();
    BSP_LED_Init();

    // Application Task Init
    Scheduler_Register(&AppSystem_heartbeat_task);
    Scheduler_Register(&led_task);

}

void AppSystem_Main()
{
    Scheduler_Run();
}

