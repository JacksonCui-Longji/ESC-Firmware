#include "Timer.h"
#include <string.h>

#define TIMER_MAX_NUM 32

typedef enum TimerChannelState_t{
    TIMER_CHANNEL_NOT_USED = 0x00u,
    TIMER_CHANNEL_USED
}TimerChannelState;

typedef struct
{
    TimerChannelState channel_state;
    TimerState state;
    TimerType type;
    uint32_t period_ms;
    uint32_t counter_ms;
    EventId event;
}TimerChannel;

static TimerChannel s_timerTable[TIMER_MAX_NUM];

void Timer_Init(void)
{
    memset(s_timerTable, 0, sizeof(s_timerTable));

    for(uint8_t i = 0; i < TIMER_MAX_NUM; i++)
    {
        s_timerTable[i].channel_state = TIMER_CHANNEL_NOT_USED;
        s_timerTable[i].state = TIMER_STATE_STOP;
        s_timerTable[i].type = TIMER_TYPE_ONE_SHOT;
    }
}

TimerId Timer_Create(uint32_t period_ms, EventId event, TimerType type)
{
    if(period_ms == 0)
    {
        return TIMER_ID_INVALID;
    }

    if(event <= EVENT_NONE || event >= EVENT_MAX)
    {
        return TIMER_ID_INVALID;
    }

    if(type >= TIMER_TYPE_COUNT)
    {
        return TIMER_ID_INVALID;
    }

    for(uint8_t i =0; i < TIMER_MAX_NUM; i++)
    {
        if(TIMER_CHANNEL_NOT_USED == s_timerTable[i].channel_state)
        {
            s_timerTable[i].channel_state = TIMER_CHANNEL_USED;
            s_timerTable[i].event = event;
            s_timerTable[i].period_ms = period_ms;
            s_timerTable[i].counter_ms = 0;
            s_timerTable[i].state = TIMER_STATE_STOP;
            s_timerTable[i].type = type;
            return (TimerId)i;
        }
    }
    return (TimerId)TIMER_ID_INVALID;
}

ret_t Timer_Delete(TimerId id)
{
    if(id >= TIMER_MAX_NUM)
    {
        return RET_NG;
    }

    if(s_timerTable[id].channel_state != TIMER_CHANNEL_USED)
    {
        return RET_NG;
    }

    memset(&s_timerTable[id], 0x00u, sizeof(TimerChannel));
    return RET_OK;
}

ret_t Timer_Start(TimerId id)
{
    if(id >= TIMER_MAX_NUM)
    {
        return RET_NG;
    }

    if(s_timerTable[id].channel_state != TIMER_CHANNEL_USED)
    {
        return RET_NG;
    }

    s_timerTable[id].counter_ms = 0;
    s_timerTable[id].state = TIMER_STATE_RUNNING;
    return RET_OK;
}

ret_t Timer_Stop(TimerId id)
{
    if(id >= TIMER_MAX_NUM)
    {
        return RET_NG;
    }
    
    if(s_timerTable[id].channel_state != TIMER_CHANNEL_USED)
    {
        return RET_NG;
    }

    s_timerTable[id].counter_ms = 0;
    s_timerTable[id].state = TIMER_STATE_STOP;
    return RET_OK;
}

void Timer_MainFunction(uint32_t elapsed_ms)
{
    for(uint8_t i = 0; i < TIMER_MAX_NUM; i++)
    {
        TimerChannel *timer = &s_timerTable[i];

        if(timer->channel_state != TIMER_CHANNEL_USED)
        {
            continue;
        }

        if(timer->state != TIMER_STATE_RUNNING)
        {
            continue;
        }

        timer->counter_ms += elapsed_ms;

        while(timer->counter_ms >= timer->period_ms)
        {
            timer->counter_ms -= timer->period_ms;
            if(Event_Post(timer->event) != RET_OK)
            {
                // log error
            }
            if(TIMER_TYPE_ONE_SHOT == timer->type)
            {
                timer->state = TIMER_STATE_STOP;
                timer->counter_ms = 0;
                break;
            }
            else
            {
                // timer->state = TIMER_STATE_RUNNING;
            }
        }
    }
}
