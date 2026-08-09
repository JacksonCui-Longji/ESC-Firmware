#define LOG_MODULE ("BSP_UART")

#include "Logger_Macro.h"
#include "BSP_UART.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_dma.h"
#include "main.h"
#include <string.h>
#include <stdbool.h>

#define DMA_UART

extern UART_HandleTypeDef huart1;

#ifndef DMA_UART
static uint8_t g_uart_rx_byte;
#endif

/* ============================
 * UART RX Ring Buffer
 * ============================ */

static uint8_t g_uart_rx_buffer[BSP_UART_RX_BUFFER_MAX];

static uint16_t g_uart_rx_write_index = 0;
static uint16_t g_uart_rx_read_index = 0;

#ifdef DMA_UART
static uint16_t g_uart_rx_last_read_index = 0;
static volatile uint8_t  g_uart_rx_dma_wrap_count = 0;

static bool g_uart_rx_overflow = false;
static bool g_uart_rx_overflow_reported = false;
#endif

void BSP_UART_Init(void)
{
    // huart1 init by MX_USART1_UART_Init in main.c.
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

#ifndef DMA_UART
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
#endif

/*
 * Pop one byte from RX buffer
 *
 * called from main loop
 */
int BSP_UART_RxPop(uint8_t *data)
{
#ifdef DMA_UART
    uint16_t ndtr = __HAL_DMA_GET_COUNTER(huart1.hdmarx);
    g_uart_rx_write_index = BSP_UART_RX_BUFFER_MAX - ndtr;

    if((g_uart_rx_write_index == g_uart_rx_read_index) && (0 == g_uart_rx_dma_wrap_count))
    {
        return -1;
    }
    /*
     * DMA has caught up with the CPU while at least one
     * DMA wrap is still outstanding.
     *
     * This means DMA has overwritten data which has not
     * yet been consumed by CPU.
     */
    if((g_uart_rx_dma_wrap_count > 0u) && (g_uart_rx_write_index == g_uart_rx_read_index))
    {
        if(!g_uart_rx_overflow)
        {
            g_uart_rx_overflow = true;
            if(!g_uart_rx_overflow_reported)
            {
                g_uart_rx_overflow_reported = true;
                LOG_WARN("DMA over flow detected!\r\n");
            }
        }
    }

#endif

    *data = g_uart_rx_buffer[g_uart_rx_read_index];
    g_uart_rx_read_index = (g_uart_rx_read_index + 1) % BSP_UART_RX_BUFFER_MAX;

#ifdef DMA_UART
    if(g_uart_rx_read_index < g_uart_rx_last_read_index)
    {
        if(g_uart_rx_dma_wrap_count > 0u)
        {
            g_uart_rx_dma_wrap_count--;
        }
    }

    g_uart_rx_last_read_index = g_uart_rx_read_index;
    /*
     * Check whether CPU has caught up with DMA after
     * an overflow condition.
     *
     * When the outstanding DMA wrap count becomes zero,
     * CPU has consumed enough data to recover.
     */
    if(g_uart_rx_overflow && (g_uart_rx_dma_wrap_count == 0u))
    {
        g_uart_rx_overflow = false;
        g_uart_rx_overflow_reported = false;

        LOG_WARN("DMA over flow recovery!\r\n");
    }

#endif

    return 0;
}

static void BSP_UART_DmaRxWrap(void)
{
    if(g_uart_rx_dma_wrap_count < 0xFFu)
    {
        g_uart_rx_dma_wrap_count++;
    }
}

void BSP_UART_StartRx(void)
{
    memset(g_uart_rx_buffer, 0x00u, sizeof(g_uart_rx_buffer));

    g_uart_rx_write_index = 0;
    g_uart_rx_read_index = 0;

#ifdef DMA_UART

    g_uart_rx_last_read_index = 0;
    g_uart_rx_dma_wrap_count = 0;

    g_uart_rx_overflow = false;
    g_uart_rx_overflow_reported = false;

    if (HAL_UART_Receive_DMA(&huart1, g_uart_rx_buffer, BSP_UART_RX_BUFFER_MAX) != HAL_OK)
    {
        Error_Handler();
    }
#else

    HAL_UART_Receive_IT(&huart1, &g_uart_rx_byte, 1);

#endif
}

/*
 * STM32 HAL UART RX complete callback.
 *
 * In DMA circular mode, one callback means the DMA RX buffer
 * has completed one full cycle.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
#ifdef DMA_UART
        BSP_UART_DmaRxWrap();
#else
        BSP_UART_RxPush(g_uart_rx_byte);
        /*
         * restart RX interrupt
         */
        HAL_UART_Receive_IT(&huart1, &g_uart_rx_byte, 1);
#endif   
    }
}
