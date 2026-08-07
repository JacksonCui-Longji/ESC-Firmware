#include "Scheduler.h"
#include "BSP_Delay.h"
#include "BSP_Time.h"
#include "CLI.h"
#include "Timer.h"
#include "SystemTime.h"
#include <string.h>

#define MAX_TASK_NUM 10

static Task_t *task_list[MAX_TASK_NUM];
static uint32_t task_count = 0;

void Scheduler_Init(void)
{
    memset(task_list, 0, sizeof(task_list));
    task_count = 0;
}

ret_t Scheduler_Register(Task_t *task)
{
    if(task == NULL)
    {
        return RET_NG;
    }
    
    if(task_count >= MAX_TASK_NUM)
    {
        return RET_NG;
    }

    task_list[task_count++] = task;

    return RET_OK;
}

void Scheduler_MainFunction(void)
{
    uint32_t elapsed_ms;

    elapsed_ms = SystemTime_GetElapsedMs();
    Timer_MainFunction(elapsed_ms);
    CLI_MainFunction();
    for(uint32_t i = 0;i < task_count; i++)
    {
        Task_t *task = task_list[i];
        task->function();
    }
}