#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "esc_types.h"

typedef void (*TaskFunc)(void);

typedef struct
{
    const char *name;
    TaskFunc function;
    uint32_t period_ms;
    uint32_t last_run;
} Task_t;

void Scheduler_Init(void);
void Scheduler_Run(void);
int Scheduler_Register(Task_t *task);


#endif // SCHEDULER_H