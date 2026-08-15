#ifndef BSP_LCD_H
#define BSP_LCD_H

#include "esc_types.h"


#define BSP_LCD_WIDTH     (240u)
#define BSP_LCD_HEIGHT    (320u)

/*
    eg for RGB565
    15                         0
    ┌─────────┬──────────────┬─────────┐
    │ R[4:0]  │    G[5:0]    │ B[4:0] │
    └─────────┴──────────────┴─────────┘

    0xF800:
    1111 1000 0000 0000
    
    11111 000000 00000
    R       G       B
*/
#define BSP_LCD_RGB888_TO_RGB565(r, g, b) \
    ((((uint16_t)(r) >> 3) << 11) | \
     (((uint16_t)(g) >> 2) << 5)  | \
     ((uint16_t)(b) >> 3))


extern void BSP_LCD_Init(void);
extern void BSP_LCD_BacklightOn(void);
extern void BSP_LCD_BacklightOff(void);
extern void BSP_LCD_Fill(uint16_t color);
extern void BSP_LCD_FillRect(uint16_t x_start, uint16_t y_start, 
                             uint16_t x_end, uint16_t y_end, 
                             uint16_t color);
extern void BSP_LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
extern void BSP_LCD_Reset(void);

#endif // BSP_LCD_H