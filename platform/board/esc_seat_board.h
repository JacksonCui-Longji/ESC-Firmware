#ifndef ESC_SEAT_BOARD_H
#define ESC_SEAT_BOARD_H


#include "stm32f1xx_hal.h"


/*
 * ESC Seat Board Hardware Configuration
 *
 * MCU:
 * STM32F103VET6
 *
 */


/* =========================
 * LED Configuration
 * ========================= */


/* Green LED */
#define BOARD_LED_GREEN_GPIO_PORT    GPIOB
#define BOARD_LED_GREEN_GPIO_PIN     GPIO_PIN_0


/* Blue LED */
#define BOARD_LED_BLUE_GPIO_PORT     GPIOB
#define BOARD_LED_BLUE_GPIO_PIN      GPIO_PIN_1


/* Red LED */
#define BOARD_LED_RED_GPIO_PORT      GPIOB
#define BOARD_LED_RED_GPIO_PIN       GPIO_PIN_5



#endif /* ESC_SEAT_BOARD_H */