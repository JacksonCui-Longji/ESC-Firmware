#ifndef BSP_LCD_H
#define BSP_LCD_H

#include "esc_types.h"


extern void BSP_LCD_Init(void);
extern void BSP_LCD_Fill(uint16_t color);
extern void BSP_LCD_BL();
extern void BSP_LCD_WriteCommand(uint16_t cmd);
extern void BSP_LCD_WriteData(uint16_t data);
extern void BSP_LCD_Reset(void);

#endif // BSP_LCD_H