#include "GUI_Font_8x16.h"


static const uint8_t GUI_Font_8x16_Space[16] =
{
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};


static const uint8_t GUI_Font_8x16_A[16] =
{
    0x18,
    0x3C,
    0x66,
    0x66,
    0x66,
    0x7E,
    0x66,
    0x66,
    0x66,
    0x66,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
};


static const uint8_t GUI_Font_8x16_C[16] =
{
    0x3C,
    0x66,
    0xC0,
    0xC0,
    0xC0,
    0xC0,
    0xC0,
    0xC0,
    0x66,
    0x3C,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
};


static const uint8_t GUI_Font_8x16_E[16] =
{
    0x7E,
    0x60,
    0x60,
    0x60,
    0x60,
    0x7C,
    0x60,
    0x60,
    0x60,
    0x7E,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
};


static const uint8_t GUI_Font_8x16_S[16] =
{
    0x3C,
    0x66,
    0x60,
    0x60,
    0x3C,
    0x06,
    0x06,
    0x06,
    0x66,
    0x3C,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
};


static const uint8_t GUI_Font_8x16_T[16] =
{
    0x7E,
    0x18,
    0x18,
    0x18,
    0x18,
    0x18,
    0x18,
    0x18,
    0x18,
    0x18,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
};


/*
 * Glyph objects
 */

static const GUI_Glyph GUI_Glyph_Space =
{
    .width   = 8,
    .height  = 16,
    .advance = 8,
    .bitmap  = GUI_Font_8x16_Space
};


static const GUI_Glyph GUI_Glyph_A =
{
    .width   = 8,
    .height  = 16,
    .advance = 8,
    .bitmap  = GUI_Font_8x16_A
};


static const GUI_Glyph GUI_Glyph_C =
{
    .width   = 8,
    .height  = 16,
    .advance = 8,
    .bitmap  = GUI_Font_8x16_C
};


static const GUI_Glyph GUI_Glyph_E =
{
    .width   = 8,
    .height  = 16,
    .advance = 8,
    .bitmap  = GUI_Font_8x16_E
};


static const GUI_Glyph GUI_Glyph_S =
{
    .width   = 8,
    .height  = 16,
    .advance = 8,
    .bitmap  = GUI_Font_8x16_S
};


static const GUI_Glyph GUI_Glyph_T =
{
    .width   = 8,
    .height  = 16,
    .advance = 8,
    .bitmap  = GUI_Font_8x16_T
};


/*
 * Codepoint -> Glyph table
 */

static const GUI_FontGlyph GUI_Font_8x16_Glyphs[] =
{
    {
        .codepoint = ' ',
        .glyph     = &GUI_Glyph_Space
    },

    {
        .codepoint = 'A',
        .glyph     = &GUI_Glyph_A
    },

    {
        .codepoint = 'C',
        .glyph     = &GUI_Glyph_C
    },

    {
        .codepoint = 'E',
        .glyph     = &GUI_Glyph_E
    },

    {
        .codepoint = 'S',
        .glyph     = &GUI_Glyph_S
    },

    {
        .codepoint = 'T',
        .glyph     = &GUI_Glyph_T
    }
};


const GUI_Font GUI_Font_8x16 =
{
    .height     = 16,
    .glyph_count = sizeof(GUI_Font_8x16_Glyphs) /
                   sizeof(GUI_Font_8x16_Glyphs[0]),
    .glyphs     = GUI_Font_8x16_Glyphs
};