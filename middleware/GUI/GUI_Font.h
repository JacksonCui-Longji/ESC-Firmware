#ifndef GUI_FONT_H
#define GUI_FONT_H

#include "esc_types.h"


typedef struct
{
    uint16_t width;
    uint16_t height;
    uint16_t advance;

    const uint8_t *bitmap;

} GUI_Glyph;


typedef struct
{
    uint32_t codepoint;

    const GUI_Glyph *glyph;

} GUI_FontGlyph;


typedef struct
{
    uint16_t height;

    uint16_t glyph_count;

    const GUI_FontGlyph *glyphs;

} GUI_Font;


/*
 * Built-in fonts
 */

extern const GUI_Font GUI_Font_8x16;


#endif // GUI_FONT_H