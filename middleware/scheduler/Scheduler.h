#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "esc_types.h"

typedef void (*TaskFunc)(void);

typedef struct
{
    const char *name;
    TaskFunc function;
} Task_t;

void Scheduler_Init(void);
void Scheduler_MainFunction(void);
ret_t Scheduler_Register(Task_t *task);


#endif // SCHEDULER_H