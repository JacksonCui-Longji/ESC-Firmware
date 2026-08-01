#ifndef BSP_LED_H
#define BSP_LED_H

#include  "esc_types.h"

typedef enum BSP_LED_Color_t {
    BSP_LED_COLOR_GREEN = 0x00u,
    BSP_LED_COLOR_BLUE,
    BSP_LED_COLOR_RED,
}BSP_LED_Color;

typedef enum BSP_LED_State_t {
    BSP_LED_INVALID = 0x00u,
    BSP_LED_OFF,
    BSP_LED_ON,
}BSP_LED_State;

/* extern interface */
extern ret_t BSP_LED_Init(void);
extern ret_t BSP_LED_Deinit(void);
extern ret_t BSP_LED_State_Set(BSP_LED_Color led, BSP_LED_State state);
extern BSP_LED_State BSP_LED_State_Get(BSP_LED_Color led);
extern ret_t BSP_LED_Toggle(BSP_LED_Color led);

#endif // BSP_LED_H
