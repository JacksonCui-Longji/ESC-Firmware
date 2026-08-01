#ifndef EVENT_H
#define EVENT_H

#include "esc_types.h"

typedef uint32_t EventBitMap; // uint32 for stm32 platform

typedef enum {
    EVENT_NONE = 0,
    EVENT_LED,
    EVENT_CAN_RX,
    EVENT_CAN_TX,
    EVENT_MOTOR_FINISH,
    EVENT_TIMEOUT,
    EVENT_DIAG_REQUEST,
    EVENT_MAX
}EventId;

typedef enum{
    EVENT_STATE_NOT_PENDING = 0x00u,
    EVENT_STATE_PENDING
}EventPendingState;

extern void Event_Init(void);
extern ret_t Event_Post(EventId id);
extern ret_t Event_Clear(EventId id);
extern EventPendingState Event_IsPending(EventId id);
extern EventBitMap Event_PendingGet(void);

#endif // EVENT_H
