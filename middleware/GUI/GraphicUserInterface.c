#include "GraphicUserInterface.h"
#include "BSP_LCD.h"

void GUI_Init(void)
{
    BSP_LCD_Init();
    BSP_LCD_Fill(0xF800);
    BSP_LCD_BL();
}