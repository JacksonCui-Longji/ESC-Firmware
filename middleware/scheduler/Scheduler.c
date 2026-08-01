#include "Scheduler.h"
#include "BSP_Delay.h"
#include "BSP_Time.h"


#define MAX_TASK_NUM 10

static Task_t *task_list[MAX_TASK_NUM];
static uint32_t task_count = 0;

void Scheduler_Init(void)
{

}

int Scheduler_Register(Task_t *task)
{
    if(task_count >= MAX_TASK_NUM)
    {
        return -1;
    }

    task_list[task_count++] = task;

    return 0;
}

void Scheduler_Run(void)
{
    uint32_t now = BSP_GetTick();

    for(uint32_t i=0;i<task_count;i++)
    {
        Task_t *task = task_list[i];
        if(now - task->last_run >= task->period_ms)
        {
            task->last_run = now;
            task->function();
        }
    }
}