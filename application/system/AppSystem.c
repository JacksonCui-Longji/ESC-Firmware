#include "AppSystem.h"
#include "BSP_LED.h"
#include "BSP_Delay.h"
#include "BSP_UART.h"
#include "Scheduler.h"
#include "Event.h"
#include "Timer.h"
#include "AppLed.h"

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
    // Driver Init
    BSP_Delay_Init();
    BSP_LED_Init();
    BSP_UART_Init();
    BSP_UART_StartRx();
    
    // Middleware Init
    Event_Init();
    Scheduler_Init();
    Timer_Init();
    
    // Application Task Init
    Scheduler_Register(&AppSystem_heartbeat_task);
    AppLed_Init();

}

