#ifndef BSP_STEPPER_H
#define BSP_STEPPER_H

#include "esc_types.h"
#include "stm32f1xx_hal.h"

/*
 * Stepper hardware mapping
 *
 * These definitions are intentionally kept here, rather than in a generic
 * BSP_GPIO layer.  Change this block when the ESC board wiring changes.
 *
 * Default mapping (STM32F103VET6):
 *   PB6 / TIM4_CH1 -> driver STEP/PUL input
 *   PB7            -> driver DIR input
 *   PB8            -> driver ENABLE input
 *
 * PB6, PB7 and PB8 are currently unused by the generated CubeMX project.
 */
#define BSP_STEPPER_STEP_GPIO_PORT          GPIOB
#define BSP_STEPPER_STEP_GPIO_PIN           GPIO_PIN_6
#define BSP_STEPPER_STEP_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_STEPPER_STEP_TIMER              TIM4
#define BSP_STEPPER_STEP_TIMER_CLK_ENABLE() __HAL_RCC_TIM4_CLK_ENABLE()
#define BSP_STEPPER_STEP_TIMER_CHANNEL      TIM_CHANNEL_1

#define BSP_STEPPER_DIR_GPIO_PORT           GPIOB
#define BSP_STEPPER_DIR_GPIO_PIN            GPIO_PIN_7
#define BSP_STEPPER_DIR_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

#define BSP_STEPPER_ENABLE_GPIO_PORT        GPIOB
#define BSP_STEPPER_ENABLE_GPIO_PIN         GPIO_PIN_8
#define BSP_STEPPER_ENABLE_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

/* Driver electrical-level configuration.  Adjust to match the actual driver wiring. */
#define BSP_STEPPER_ENABLE_ACTIVE_STATE     GPIO_PIN_RESET
#define BSP_STEPPER_ENABLE_INACTIVE_STATE   GPIO_PIN_SET
#define BSP_STEPPER_DIR_FORWARD_STATE       GPIO_PIN_SET
#define BSP_STEPPER_DIR_BACKWARD_STATE      GPIO_PIN_RESET

/* TIM4 runs from this clock with the current 8 MHz HSI system configuration. */
#define BSP_STEPPER_TIMER_CLOCK_HZ          (8000000u)
#define BSP_STEPPER_DEFAULT_FREQUENCY_HZ    (1000u)

typedef enum BSP_Stepper_Direction_t
{
    BSP_STEPPER_FORWARD = 0u,
    BSP_STEPPER_BACKWARD
} BSP_Stepper_Direction;

ret_t BSP_Stepper_Init(void);
ret_t BSP_Stepper_Enable(void);
ret_t BSP_Stepper_Disable(void);
ret_t BSP_Stepper_SetDirection(BSP_Stepper_Direction direction);
ret_t BSP_Stepper_SetFrequency(uint32_t frequency_hz);
ret_t BSP_Stepper_Start(void);
ret_t BSP_Stepper_Stop(void);

#endif /* BSP_STEPPER_H */
