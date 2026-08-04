#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "esc_types.h"

typedef void (*TaskFunc)(void);

typedef struct
{
    const char *name;
    TaskFunc function;
} Task_t;

extern void Scheduler_Init(void);
extern void Scheduler_MainFunction(void);
extern ret_t Scheduler_Register(Task_t *task);


#endif // SCHEDULER_H