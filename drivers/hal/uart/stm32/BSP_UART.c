#include "BSP_UART.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include <string.h>


extern UART_HandleTypeDef huart1;

static uint8_t g_uart_rx_byte;


/* ============================
 * UART RX Ring Buffer
 * ============================ */

static uint8_t g_uart_rx_buffer[BSP_UART_RX_BUFFER_MAX];

static uint16_t g_uart_rx_write_index = 0;
static uint16_t g_uart_rx_read_index = 0;



void BSP_UART_Init(void)
{
    // MX_USART1_UART_Init already finish below init.
    // huart1.Instance = USART1;
    // huart1.Init.BaudRate = 115200;
    // huart1.Init.WordLength = UART_WORDLENGTH_8B;
    // huart1.Init.StopBits = UART_STOPBITS_1;
    // huart1.Init.Parity = UART_PARITY_NONE;
    // huart1.Init.Mode = UART_MODE_TX_RX;
    // huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    // huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    // if (HAL_UART_Init(&huart1) != HAL_OK)
    // {
    //     // Error_Handler();
    // }
}

int BSP_UART_Send(const uint8_t *data, uint16_t len)
{
    return HAL_UART_Transmit(
            &huart1,
            (uint8_t *)data,
            len,
            1000
    );
}

/*
 * Push one byte into RX buffer
 *
 * called from ISR
 */
static void BSP_UART_RxPush(uint8_t data)
{
    uint16_t next;


    next = (g_uart_rx_write_index + 1) % BSP_UART_RX_BUFFER_MAX;

    if(next == g_uart_rx_read_index)
    {
        /*
         * overflow
         * drop new data
         */
        return;
    }

    g_uart_rx_buffer[g_uart_rx_write_index] = data;
    g_uart_rx_write_index = next;
}



/*
 * Pop one byte from RX buffer
 *
 * called from main loop
 */
int BSP_UART_RxPop(uint8_t *data)
{
    if(g_uart_rx_write_index == g_uart_rx_read_index)
    {
        return -1;
    }


    *data = g_uart_rx_buffer[g_uart_rx_read_index];
    g_uart_rx_read_index = (g_uart_rx_read_index + 1) % BSP_UART_RX_BUFFER_MAX;

    return 0;
}



void BSP_UART_StartRx(void)
{
    memset(g_uart_rx_buffer, 0x00u, sizeof(g_uart_rx_buffer));

    g_uart_rx_write_index = 0;
    g_uart_rx_read_index = 0;

    // HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
    HAL_UART_Receive_IT(&huart1, &g_uart_rx_byte, 1);
}

/*
 * STM32 HAL callback
 *
 * one byte received
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        BSP_UART_RxPush(g_uart_rx_byte);
        /*
         * restart RX interrupt
         */
        HAL_UART_Receive_IT(&huart1, &g_uart_rx_byte, 1);
    }
}
