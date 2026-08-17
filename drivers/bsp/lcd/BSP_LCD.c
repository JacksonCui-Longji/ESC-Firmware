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

#define BSP_LCD_CMD_SOFT_RST                (0x01u)
#define BSP_LCD_CMD_SLEEP_OUT               (0x11u)
#define BSP_LCD_CMD_PIXEL_16BIT_RGB565      (0x3Au)
#define BSP_LCD_CMD_MADCTL                  (0x36u)
#define BSP_LCD_CMD_DISPLAY_ON              (0x29u)
#define BSP_LCD_CMD_COLU_ADDR_SET           (0x2Au)
#define BSP_LCD_CMD_PAGE_ADDR_SET           (0x2Bu)
#define BSP_LCD_CMD_COLOR_SET               (0x2Cu)


static void BSP_LCD_WriteCommand(uint16_t cmd)
{
    BSP_LCD_CMD = cmd;
}

static void BSP_LCD_WriteData(uint16_t data)
{
    BSP_LCD_DATA = data;
}

static void BSP_LCD_SetAddressWindow(uint16_t x_start,
                                     uint16_t y_start,
                                     uint16_t x_end,
                                     uint16_t y_end)
{
    BSP_LCD_WriteCommand(BSP_LCD_CMD_COLU_ADDR_SET);

    /*
    * ILI9341 address parameters are 16-bit values,
    * transmitted as two consecutive 8-bit parameters:
    *
    *   High byte -> Low byte
    */
    // send high 8 bits
    BSP_LCD_WriteData(x_start >> 8);
    // send low 8 bits
    BSP_LCD_WriteData(x_start & 0xFF);
    BSP_LCD_WriteData(x_end >> 8);
    BSP_LCD_WriteData(x_end & 0xFF);

    BSP_LCD_WriteCommand(BSP_LCD_CMD_PAGE_ADDR_SET);

    BSP_LCD_WriteData(y_start >> 8);
    BSP_LCD_WriteData(y_start & 0xFF);
    BSP_LCD_WriteData(y_end >> 8);
    BSP_LCD_WriteData(y_end & 0xFF);
}

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
    BSP_LCD_WriteCommand(BSP_LCD_CMD_SOFT_RST);
    HAL_Delay(120);

    // ILI9341 sleep out
    BSP_LCD_WriteCommand(BSP_LCD_CMD_SLEEP_OUT);
    HAL_Delay(120);

    /* Pixel Format: 16-bit/pixel RGB565 */
    BSP_LCD_WriteCommand(BSP_LCD_CMD_PIXEL_16BIT_RGB565);
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
    BSP_LCD_WriteCommand(BSP_LCD_CMD_MADCTL);
    BSP_LCD_WriteData(0x88);

    /* Display ON */
    BSP_LCD_WriteCommand(BSP_LCD_CMD_DISPLAY_ON);
    HAL_Delay(20);
}

void BSP_LCD_BacklightOn(void)
{
    HAL_GPIO_WritePin(BOARD_LCD_BL_GPIO_PORT,
                      BOARD_LCD_BL_GPIO_PIN,
                      GPIO_PIN_RESET);
}

void BSP_LCD_BacklightOff(void)
{
    HAL_GPIO_WritePin(BOARD_LCD_BL_GPIO_PORT,
                      BOARD_LCD_BL_GPIO_PIN,
                      GPIO_PIN_SET);
}

void BSP_LCD_Fill(uint16_t color)
{
    uint32_t pixel_count;

    BSP_LCD_SetAddressWindow(0,
                             0,
                             BSP_LCD_WIDTH - 1,
                             BSP_LCD_HEIGHT - 1);

    BSP_LCD_WriteCommand(BSP_LCD_CMD_COLOR_SET);

    pixel_count = BSP_LCD_WIDTH * BSP_LCD_HEIGHT;

    while (pixel_count--)
    {
        BSP_LCD_WriteData(color);
    }
}

void BSP_LCD_FillRect(uint16_t x_start, uint16_t y_start, 
                      uint16_t x_end, uint16_t y_end, 
                      uint16_t color)
{
    uint32_t pixel = 0;

    if(((x_start >= BSP_LCD_WIDTH)&&(x_end >= BSP_LCD_WIDTH)) || 
        ((y_start >= BSP_LCD_HEIGHT)&&(y_end >= BSP_LCD_HEIGHT)))
    {
        return;
    }

    if(x_start >= BSP_LCD_WIDTH)
    {

        x_start = BSP_LCD_WIDTH-1;
    }

    if(y_start >= BSP_LCD_HEIGHT)
    {
        y_start = BSP_LCD_HEIGHT-1;
    }

    if(x_end >= BSP_LCD_WIDTH)
    {

        x_end = BSP_LCD_WIDTH-1;
    }

    if(y_end >= BSP_LCD_HEIGHT)
    {
        y_end = BSP_LCD_HEIGHT-1;
    }

    if(x_start > x_end)
    {
        uint16_t tmp = x_start;
        x_start = x_end;
        x_end = tmp;
    }

    if(y_start > y_end)
    {
        uint16_t tmp = y_start;
        y_start = y_end;
        y_end = tmp;
    }

    pixel = (x_end-x_start+1)*(y_end-y_start+1);

    BSP_LCD_SetAddressWindow(x_start, y_start, 
                             x_end, y_end);

    BSP_LCD_WriteCommand(BSP_LCD_CMD_COLOR_SET);

    while(pixel--)
    {
        BSP_LCD_WriteData(color);
    }
    return;
}

void BSP_LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if((x >= BSP_LCD_WIDTH) || (y >= BSP_LCD_HEIGHT))
    {
        return;
    }

    BSP_LCD_SetAddressWindow(x, y, x, y);
    BSP_LCD_WriteCommand(BSP_LCD_CMD_COLOR_SET);
    BSP_LCD_WriteData(color);

}

void BSP_LCD_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *image)
{
    uint32_t pixel_count;

    if (image == NULL)
    {
        return;
    }

    if ((x >= BSP_LCD_WIDTH) || (y >= BSP_LCD_HEIGHT))
    {
        return;
    }

    if ((width == 0) || (height == 0))
    {
        return;
    }

    if ((x + width > BSP_LCD_WIDTH) || (y + height > BSP_LCD_HEIGHT))
    {
        return;
    }

    BSP_LCD_SetAddressWindow(x, y, x + width - 1, y + height - 1);
    BSP_LCD_WriteCommand(BSP_LCD_CMD_COLOR_SET);

    pixel_count = (uint32_t)width * height;

    while (pixel_count--)
    {
        BSP_LCD_WriteData(*image++);
    }
}
