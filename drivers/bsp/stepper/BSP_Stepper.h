#ifndef BSP_STEPPER_H
#define BSP_STEPPER_H

#include  "esc_types.h"

typedef enum BSP_Stepper_Direction_t{
    BSP_STEPPER_STOP,
    BSP_STEPPER_FORWARD,
    BSP_STEPPER_BACKWARD

}BSP_Stepper_Direction;


extern void BSP_Stepper_Init(void);

extern void BSP_Stepper_Enable(void);

extern void BSP_Stepper_Disable(void);

extern void BSP_Stepper_SetDirection(BSP_Stepper_Direction direction);

extern void BSP_Stepper_Step(void);

extern void BSP_Stepper_Stop(void);

#endif