#include "BSP_UART.h"
#include "stm32f1xx_hal.h"
#include "main.h"

static UART_HandleTypeDef g_uart1_handle;

void BSP_UART_Init(void)
{
  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  g_uart1_handle.Instance = USART1;
  g_uart1_handle.Init.BaudRate = 115200;
  g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;
  g_uart1_handle.Init.StopBits = UART_STOPBITS_1;
  g_uart1_handle.Init.Parity = UART_PARITY_NONE;
  g_uart1_handle.Init.Mode = UART_MODE_TX_RX;
  g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  g_uart1_handle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&g_uart1_handle) != HAL_OK)
  {
    // Error_Handler(); // in stm32 main.h
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}

int BSP_UART_Send(const uint8_t *data, uint16_t len)
{
    return HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)data, len, 1000);
}