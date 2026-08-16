#include "GraphicUserInterface.h"
#include "BSP_LCD.h"
#include "GUI_Color.h"

void GUI_Init(void)
{
    BSP_LCD_Init();
    GUI_Clear();
    BSP_LCD_BacklightOn();

    uint16_t image[] = {
        GUI_COLOR_RED, GUI_COLOR_RED, GUI_COLOR_GREEN, GUI_COLOR_GREEN,
        GUI_COLOR_RED, GUI_COLOR_RED, GUI_COLOR_GREEN, GUI_COLOR_GREEN,
        GUI_COLOR_BLUE, GUI_COLOR_BLUE, GUI_COLOR_WHITE, GUI_COLOR_WHITE,
        GUI_COLOR_BLUE, GUI_COLOR_BLUE, GUI_COLOR_WHITE, GUI_COLOR_WHITE,
    };

    BSP_LCD_DrawImage(60, 80, 
                      4, 4, 
                      image);
}

void GUI_Clear(void)
{
    BSP_LCD_Fill(GUI_COLOR_WHITE);
}

void GUI_DrawRect(uint16_t x_start, uint16_t y_start, 
                  uint16_t x_end, uint16_t y_end, 
                  uint16_t color)
{
    BSP_LCD_FillRect(x_start, y_start, 
                     x_end, y_end, 
                     color);
}
