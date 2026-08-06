#ifndef BSP_HAL_UART_H
#define BSP_HAL_UART_H

#include <stdint.h>

extern void BSP_UART_Init(void);
extern int BSP_UART_Send(const uint8_t *data, uint16_t len);
extern void BSP_UART_StartRx(void);

#endif