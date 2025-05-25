// font8x8_basic.c
#include "font8x8_basic.h"
#include <string.h>
#include "st7701s_display.h"

#ifdef __cplusplus
extern "C" {
#endif



void draw_char_zoomed(int x, int y, char c, int scale) {
   if (c > 127) return;
    const uint8_t *glyph = font8x8_basic[(int)c];

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (glyph[row] & (1 << col)) {
                for (int dx = 0; dx < scale; dx++) {
                    for (int dy = 0; dy < scale; dy++) {
                        display_draw_pixel(x + col * scale + dx, y + row * scale + dy, 1);
                    }
                }
            }
        }
    }
}

#ifdef __cplusplus
}
#endif