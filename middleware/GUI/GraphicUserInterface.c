#include "GraphicUserInterface.h"
#include "BSP_LCD.h"

/* GUI color definitions */

#define GUI_COLOR_BLACK       BSP_LCD_RGB888_TO_RGB565(0,   0,   0)
#define GUI_COLOR_WHITE       BSP_LCD_RGB888_TO_RGB565(255, 255, 255)
#define GUI_COLOR_BACKGROUND  BSP_LCD_RGB888_TO_RGB565(30,  30,  30)
#define GUI_COLOR_TEXT        BSP_LCD_RGB888_TO_RGB565(255, 255, 255)
#define GUI_COLOR_WARNING     BSP_LCD_RGB888_TO_RGB565(255, 180, 0)

void GUI_Init(void)
{
    BSP_LCD_Init();
    BSP_LCD_Fill(BSP_LCD_RGB888_TO_RGB565(43, 170, 91));
    BSP_LCD_BL();
}