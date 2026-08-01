#ifndef TIMER_H
#define TIMER_H

#include "esc_types.h"
#include "Event.h"

#define TIMER_ID_INVALID (0xffu)

typedef uint8_t TimerId;


typedef enum TimerState_t{
    TIMER_STATE_STOP = 0x00u,
    TIMER_STATE_RUNNING

}TimerState;

typedef enum TimerType_t{
    TIMER_TYPE_ONE_SHOT = 0x00u,
    TIMER_TYPE_PERIODIC,
    TIMER_TYPE_COUNT
}TimerType;

extern void Timer_Init(void);

extern TimerId Timer_Create(uint32_t period_ms, EventId event, TimerType type);
extern ret_t Timer_Delete(TimerId id);
extern ret_t Timer_Start(TimerId id);
extern ret_t Timer_Stop(TimerId id);

extern void Timer_MainFunction(uint32_t elapsed_ms);

#endif // TIMER_H