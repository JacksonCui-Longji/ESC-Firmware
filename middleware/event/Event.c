#include "Event.h"

static EventBitMap s_eventBitmap = 0u;

void Event_Init(void)
{
    s_eventBitmap = 0u;
}

ret_t Event_Post(EventId id)
{
    if(id <= EVENT_NONE || id >= EVENT_MAX)
    {
        return RET_NG;
    }
    s_eventBitmap |= (1UL << id);
    return RET_OK;
}

ret_t Event_Clear(EventId id)
{
    if(id <= EVENT_NONE || id >= EVENT_MAX)
    {
        return RET_NG;
    }
    s_eventBitmap &= ~(1UL << id);
    return RET_OK;
}

EventPendingState Event_IsPending(EventId id)
{
    if(id <= EVENT_NONE || id >= EVENT_MAX)
    {
        return EVENT_STATE_NOT_PENDING;
    }

    if(s_eventBitmap & (1UL << id))
    {
        return EVENT_STATE_PENDING;
    }

    return EVENT_STATE_NOT_PENDING;
}

EventBitMap Event_PendingGet(void)
{
    return s_eventBitmap;
}

