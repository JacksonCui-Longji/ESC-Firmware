#include "GraphicUserInterface.h"
#include "BSP_LCD.h"
#include "GUI_Color.h"
// TEST HEAD FILES START
#include "GUI_Image_Seat.h"
#include "GUI_Text.h"
// TEST HEAD FILES END
#include "string.h"

void GUI_Init(void)
{
    BSP_LCD_Init();
    GUI_Clear();
    BSP_LCD_DrawImage(0, 0, 
                      GUI_IMAGE_SEAT_WIDTH, GUI_IMAGE_SEAT_HEIGHT, 
                      GUI_IMAGE_SEAT);
    GUI_DrawText(20,
                 20,
                 "ESC SEAT",
                 &GUI_Font_8x16,
                 GUI_COLOR_RED,
                 GUI_COLOR_WHITE);

    BSP_LCD_BacklightOn();

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
