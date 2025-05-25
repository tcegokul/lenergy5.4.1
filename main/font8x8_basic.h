// font8x8_basic.h
#ifndef FONT8X8_BASIC_H
#define FONT8X8_BASIC_H

#include <stdint.h>

extern const uint8_t font8x8_basic[128][8];
extern const uint8_t font8x8_bold[128][8];
extern void draw_char_zoomed(int x, int y, char c, int scale);

#endif // FONT8X8_BASIC_H


