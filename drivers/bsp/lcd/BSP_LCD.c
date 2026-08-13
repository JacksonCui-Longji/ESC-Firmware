#include "BSP_LCD.h"
#include "esc_seat_board.h"

/*
 * LCD RS is connected to FSMC_A16.
 *
 * 16-bit FSMC:
 * HADDR17 -> FSMC_A16 -> LCD_RS
 *
 * RS = 0 : Command
 * RS = 1 : Data
 */
#define BSP_LCD_CMD_ADDR    ((uint32_t)0x60000000U)
#define BSP_LCD_DATA_ADDR   ((uint32_t)0x60020000U)

#define BSP_LCD_CMD  (*((volatile uint16_t *)BSP_LCD_CMD_ADDR))
#define BSP_LCD_DATA (*((volatile uint16_t *)BSP_LCD_DATA_ADDR))

#define BSP_LCD_WIDTH     240U
#define BSP_LCD_HEIGHT    320U

void BSP_LCD_Reset(void)
{
    HAL_GPIO_WritePin(BOARD_LCD_GPIO_E_PORT, BOARD_LCD_RST_GPIO_PIN, GPIO_PIN_SET);
    HAL_Delay(10);

    HAL_GPIO_WritePin(BOARD_LCD_GPIO_E_PORT, BOARD_LCD_RST_GPIO_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);

    HAL_GPIO_WritePin(BOARD_LCD_GPIO_E_PORT, BOARD_LCD_RST_GPIO_PIN, GPIO_PIN_SET);
    HAL_Delay(120);
}

void BSP_LCD_Init(void)
{
    // ILI9341 hardware reset
    BSP_LCD_Reset();

    // ILI9341 software reset
    BSP_LCD_WriteCommand(0x01);
    HAL_Delay(120);

    // ILI9341 sleep out
    BSP_LCD_WriteCommand(0x11);
    HAL_Delay(120);

    /* Pixel Format: 16-bit/pixel RGB565 */
    BSP_LCD_WriteCommand(0x3A);
    BSP_LCD_WriteData(0x55);

    /* Memory Access Control (MADCTL)
    *
    * D7 MY : Row Address Order
    *         0 -> Row address increases from top to bottom
    *         1 -> Row address increases from bottom to top
    *
    * D6 MX : Column Address Order
    *         0 -> Column address increases from left to right
    *         1 -> Column address increases from right to left
    *
    * D5 MV : Row/Column Exchange
    *         0 -> MCU X/Y coordinates map to LCD column/row normally
    *         1 -> MCU X/Y coordinates are exchanged
    *              (used for display rotation / landscape orientation)
    *
    * D4 ML : Vertical Refresh Order
    *         Controls the vertical refresh direction of the LCD panel.
    *
    * D3 BGR: RGB/BGR Color Order
    *         0 -> RGB
    *         1 -> BGR
    *
    * D2 MH : Horizontal Refresh Order
    *         Controls the horizontal refresh direction of the LCD panel.
    *
    * D1:D0: Reserved / fixed at 0.
    *
    * 0x08 = 0000 1000b
    *         MY=0, MX=0, MV=0, ML=0,
    *         BGR=1, MH=0
    *
    * This configuration keeps the normal row/column addressing
    * and selects BGR color order for the LCD panel.
    */
    BSP_LCD_WriteCommand(0x36);
    BSP_LCD_WriteData(0x08);

    /* Display ON */
    BSP_LCD_WriteCommand(0x29);
    HAL_Delay(20);
}

void BSP_LCD_WriteCommand(uint16_t cmd)
{
    BSP_LCD_CMD = cmd;
}

void BSP_LCD_WriteData(uint16_t data)
{
    BSP_LCD_DATA = data;
}

static void BSP_LCD_SetAddressWindow(uint16_t x_start,
                                     uint16_t y_start,
                                     uint16_t x_end,
                                     uint16_t y_end)
{
    BSP_LCD_WriteCommand(0x2A);

    BSP_LCD_WriteData(x_start >> 8);
    BSP_LCD_WriteData(x_start & 0xFF);
    BSP_LCD_WriteData(x_end >> 8);
    BSP_LCD_WriteData(x_end & 0xFF);

    BSP_LCD_WriteCommand(0x2B);

    BSP_LCD_WriteData(y_start >> 8);
    BSP_LCD_WriteData(y_start & 0xFF);
    BSP_LCD_WriteData(y_end >> 8);
    BSP_LCD_WriteData(y_end & 0xFF);
}

void BSP_LCD_Fill(uint16_t color)
{
    uint32_t pixel_count;

    BSP_LCD_SetAddressWindow(0,
                             0,
                             BSP_LCD_WIDTH - 1,
                             BSP_LCD_HEIGHT - 1);

    BSP_LCD_WriteCommand(0x2C);

    pixel_count = BSP_LCD_WIDTH * BSP_LCD_HEIGHT;

    while (pixel_count--)
    {
        BSP_LCD_WriteData(color);
    }
}

void BSP_LCD_BL()
{
     HAL_GPIO_WritePin(BOARD_LCD_BL_GPIO_PORT,
                      BOARD_LCD_BL_GPIO_PIN,
                      GPIO_PIN_RESET);
}