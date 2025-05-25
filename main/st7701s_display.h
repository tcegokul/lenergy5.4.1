#ifndef ST7701S_DISPLAY_H
#define ST7701S_DISPLAY_H
#include <stdint.h>
#include <stdbool.h>
#include "image_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_WIDTH  240
#define LCD_HEIGHT 240

void st7701s_display_distance_status(uint8_t status);

void display_draw_image(int x, int y, int w, int h, const image_t* img);
void display_draw_icon(int x, int y, const uint16_t* icon);

void display_draw_text(int x, int y, const char* text, uint16_t color);
void display_draw_status_bar(void);
void display_render_menu(const char **menu_items, int selected);

//Dessine un pixel à l'écran
void display_draw_pixel(int x, int y, uint16_t color);  // ✅ Déjà présent
void display_draw_icon(int x, int y, const uint16_t* icon);  // ✅ À ajouter

// Initialisation
void st7701s_display_init(void);
void st7701s_display_clear(void);

// Affichages textuels
void st7701s_display_draw_text_centered(const char* text, int y, uint16_t color);

//void st7701s_display_draw_text_multiline(const char* text, int y_start, uint16_t color);
void st7701s_display_draw_text_multiline(const char* text, int y, uint16_t color);

// Affichage d’icônes
void st7701s_display_draw_icon(int x, int y, const uint16_t* icon);

// Fond d’écran
void st7701s_display_draw_custom_background(const uint16_t* bg);

// Batterie
void st7701s_display_draw_battery(int level_percent, bool charging);

// Animations
void st7701s_display_fade_intro(void);
void st7701s_display_show_boot_info(void);

#ifdef __cplusplus
}
#endif

#endif // ST7701S_DISPLAY_H
