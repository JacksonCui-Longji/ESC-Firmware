#include "GUI_Text.h"
#include "BSP_LCD.h"

#include <stdlib.h>
#include <string.h>

#define GUI_TEXT_BUFFER_WIDTH     (BSP_LCD_WIDTH)
#define GUI_TEXT_BUFFER_HEIGHT    (32u)

static uint16_t GUI_TextBuffer[
    GUI_TEXT_BUFFER_WIDTH * GUI_TEXT_BUFFER_HEIGHT
];

static const GUI_Glyph *GUI_Font_FindGlyph(const GUI_Font *font,
                                           char character)
{
    uint16_t i;

    if ((font == NULL) || (font->glyphs == NULL))
    {
        return NULL;
    }

    for (i = 0; i < font->glyph_count; i++)
    {
        if (font->glyphs[i].codepoint == (uint8_t)character)
        {
            return font->glyphs[i].glyph;
        }
    }

    return NULL;
}


static uint16_t GUI_Text_GetWidth(const char *text,
                                  const GUI_Font *font)
{
    uint32_t width = 0;
    const GUI_Glyph *glyph;

    while (*text != '\0')
    {
        glyph = GUI_Font_FindGlyph(font, *text);

        if (glyph != NULL)
        {
            width += glyph->advance;
        }

        text++;
    }

    return (uint16_t)width;
}

static void GUI_Text_RenderGlyph(uint16_t *buffer,
                                 uint16_t buffer_width,
                                 uint16_t x,
                                 uint16_t y,
                                 const GUI_Glyph *glyph,
                                 uint16_t foreground,
                                 uint16_t background)
{
    uint16_t row;
    uint16_t col;

    if ((buffer == NULL) || (glyph == NULL))
    {
        return;
    }

    for (row = 0; row < glyph->height; row++)
    {
        uint8_t bitmap = glyph->bitmap[row];
        for (col = 0; col < glyph->width; col++)
        {
            if (bitmap & (0x80u >> col))
            {
                buffer[(y + row) * buffer_width + x + col] =
                    foreground;
            }
            else
            {
                buffer[(y + row) * buffer_width + x + col] =
                    background;
            }
        }
    }
}


void GUI_DrawText(uint16_t x,
                  uint16_t y,
                  const char *text,
                  const GUI_Font *font,
                  uint16_t foreground,
                  uint16_t background)
{
    uint16_t text_width;
    uint16_t text_height;

    uint16_t cursor_x;

    const GUI_Glyph *glyph;

    uint32_t pixel_count;
    uint32_t i;


    /* =========================
     * Parameter validation
     * ========================= */

    if ((text == NULL) ||
        (font == NULL) ||
        (font->glyphs == NULL))
    {
        return;
    }

    if (*text == '\0')
    {
        return;
    }


    /* =========================
     * Calculate text size
     * ========================= */

    text_width = GUI_Text_GetWidth(text, font);
    text_height = font->height;

    if ((text_width == 0) ||
        (text_width > GUI_TEXT_BUFFER_WIDTH) ||
        (text_height > GUI_TEXT_BUFFER_HEIGHT))
    {
        return;
    }


    /* =========================
     * Check LCD boundary
     * ========================= */

    if ((x >= BSP_LCD_WIDTH) ||
        (y >= BSP_LCD_HEIGHT))
    {
        return;
    }

    if ((uint32_t)x + text_width > BSP_LCD_WIDTH)
    {
        return;
    }

    if ((uint32_t)y + text_height > BSP_LCD_HEIGHT)
    {
        return;
    }


    /* =========================
     * Clear working buffer
     * ========================= */

    pixel_count = (uint32_t)text_width * text_height;

    for (i = 0; i < pixel_count; i++)
    {
        GUI_TextBuffer[i] = background;
    }


    /* =========================
     * Render glyphs
     * ========================= */

    cursor_x = 0;

    while (*text != '\0')
    {
        glyph = GUI_Font_FindGlyph(font,
                                   (uint8_t)*text);

        if (glyph != NULL)
        {
            GUI_Text_RenderGlyph(GUI_TextBuffer,
                                 text_width,
                                 cursor_x,
                                 0,
                                 glyph,
                                 foreground,
                                 background);

            cursor_x += glyph->advance;
        }

        text++;
    }


    /* =========================
     * Draw complete text buffer
     * ========================= */

    BSP_LCD_DrawImage(x,
                      y,
                      text_width,
                      text_height,
                      GUI_TextBuffer);
}