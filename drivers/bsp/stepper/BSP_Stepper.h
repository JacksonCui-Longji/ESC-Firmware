#ifndef BSP_STEPPER_H
#define BSP_STEPPER_H

#include  "esc_types.h"

typedef enum
{
    BSP_STEPPER_STOP,
    BSP_STEPPER_FORWARD,
    BSP_STEPPER_BACKWARD

}BSP_Stepper_Direction;


extern void BSP_Stepper_Init(void);

extern void BSP_Stepper_Move(BSP_Stepper_Direction dir,uint32_t step);

extern void BSP_Stepper_Stop(void);

#endif