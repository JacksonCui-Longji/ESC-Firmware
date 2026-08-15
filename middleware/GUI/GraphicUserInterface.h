#ifndef GRAPHIC_USER_INTERFACE_H
#define GRAPHIC_USER_INTERFACE_H

#include "esc_types.h"

extern void GUI_Init(void);
extern void GUI_Clear(void);
extern void GUI_DrawRect(uint16_t x_start, uint16_t y_start, 
                         uint16_t x_end, uint16_t y_end, 
                         uint16_t color);


#endif // GRAPHIC_USER_INTERFACE_H