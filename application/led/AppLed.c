#define LOG_MODULE ("LED")

#include "AppLed.h"
#include "Event.h"
#include "BSP_LED.h"
#include "Logger_Macro.h"
#include "Timer.h"
#include "Scheduler.h"

static TimerId led_toggle_timer;

static void AppLed_Process(void);


static Task_t led_task =
{
    .name = "LED Task",
    .function = AppLed_Process,
};

void AppLed_Init(void)
{
    Scheduler_Register(&led_task);

    led_toggle_timer = Timer_Create(500, EVENT_LED, TIMER_TYPE_PERIODIC);
    Timer_Start(led_toggle_timer);

}

void AppLed_Process(void)
{
    if(EVENT_STATE_PENDING == Event_IsPending(EVENT_LED))
    {
        // BSP_LED_Toggle(BSP_LED_COLOR_BLUE);
        Event_Clear(EVENT_LED);
    }
}
