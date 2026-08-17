#ifndef GUI_TEXT_H
#define GUI_TEXT_H

#include "GUI_Font.h"

void GUI_DrawText(uint16_t x,
                  uint16_t y,
                  const char *text,
                  const GUI_Font *font,
                  uint16_t foreground,
                  uint16_t background);

#endif // GUI_TEXT_H