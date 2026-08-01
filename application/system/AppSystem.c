#include "AppSystem.h"
#include "BSP_LED.h"
#include "BSP_Delay.h"
#include "Scheduler.h"
#include "Event.h"
#include "Timer.h"

static TimerId led_toggle_timer;

static void Task_LED(void)
{
    if(EVENT_STATE_PENDING == Event_IsPending(EVENT_LED))
    {
        BSP_LED_Toggle(BSP_LED_COLOR_RED);
        Event_Clear(EVENT_LED);
    }
}

static Task_t led_task =
{
    .name = "LED Task",
    .function = Task_LED,
};

static void Task_AppSystemHeartbeat(void)
{
     // test code, further feed watchdog
}

static Task_t AppSystem_heartbeat_task = 
{
    .name = "AppSystem Heart Beat",
    .function = Task_AppSystemHeartbeat,
};

void AppSystem_Init()
{
    // Middleware Init
    Event_Init();
    Scheduler_Init();
    Timer_Init;

    // Driver Init
    BSP_Delay_Init();
    BSP_LED_Init();

    // Application Task Init
    Scheduler_Register(&AppSystem_heartbeat_task);
    Scheduler_Register(&led_task);

    led_toggle_timer = Timer_Create(500, EVENT_LED, TIMER_TYPE_PERIODIC);
    Timer_Start(led_toggle_timer);
}

