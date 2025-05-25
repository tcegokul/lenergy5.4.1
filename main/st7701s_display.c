#include "st7701s_display.h"
#include "st7701s.h"  // si besoin de fonctions bas niveau
#include "esp_lcd_panel_interface.h"  // ✅ Nécessaire pour esp_lcd_panel_draw_bitmap
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include <math.h>
#include <stdio.h>
#include "image_manager.h"
#include "prayer.h"
#include "lang.h"
#include "color.h"
#include "font8x8_basic.h" 
#include "icons.h"
#include <string.h>
#include <time.h>
#include "backgrounds.h"
#include "font8x8_basic.h"
#include "gps_parser.h"
#include "battery_monitor.h"
#include "qibla_display.h"
#include "vl53l0x_sensor.h"
#include "config_menu.h"

#define ST7701S_RST_GPIO     38
#define ST7701S_CS_GPIO      -1  // Non utilisé
#define ST7701S_PCLK_GPIO    39
#define ST7701S_DE_GPIO      41
#define ST7701S_HSYNC_GPIO   40
#define ST7701S_VSYNC_GPIO   42
#define ST7701S_DISP_EN_GPIO -1  // Facultatif ou non câblé

#define ST7701S_DATA0_GPIO   8
#define ST7701S_DATA1_GPIO   3
#define ST7701S_DATA2_GPIO   46
#define ST7701S_DATA3_GPIO   9
#define ST7701S_DATA4_GPIO   1
#define ST7701S_DATA5_GPIO   5
#define ST7701S_DATA6_GPIO   6
#define ST7701S_DATA7_GPIO   7
#define ST7701S_DATA8_GPIO   15
#define ST7701S_DATA9_GPIO   16
#define ST7701S_DATA10_GPIO  4
#define ST7701S_DATA11_GPIO  45
#define ST7701S_DATA12_GPIO  48
#define ST7701S_DATA13_GPIO  47
#define ST7701S_DATA14_GPIO  21
#define ST7701S_DATA15_GPIO  14
#define ST7701S_DATA16_GPIO  13
#define ST7701S_DATA17_GPIO  12

// Déclarations manuelles si fonctions implémentées ailleurs
float qmi8658_get_heading(void);
const char* hadith_get_by_index(int index, int lang);
void st7701s_draw_pixel(int x, int y, uint16_t color);



void display_draw_char(int x, int y, char c, uint16_t color, int font_size);

esp_err_t esp_lcd_panel_draw_bitmap(esp_lcd_panel_handle_t panel, int x_start, int y_start, int x_end, int y_end, const void *color_data);


extern esp_lcd_panel_handle_t lcd_panel_handle;

#ifdef __cplusplus
extern "C" {
#endif

   // 🖼️ Configuration du panneau RGB
esp_lcd_rgb_panel_config_t panel_config = {
    .data_width = 16,
    .bits_per_pixel = 16,
    .clk_src = LCD_CLK_SRC_DEFAULT,
    .timings = {
        .pclk_hz = 16 * 1000 * 1000,
        .h_res = 240,
        .v_res = 240,
        .hsync_pulse_width = 10,
        .hsync_back_porch = 20,
        .hsync_front_porch = 20,
        .vsync_pulse_width = 10,
        .vsync_back_porch = 20,
        .vsync_front_porch = 20,
        .flags = {
            .pclk_active_neg = false,
            .hsync_idle_low = false,
            .vsync_idle_low = false,
        },
    },
    .num_fbs = 2,
    .bounce_buffer_size_px = 0,
    .psram_trans_align = 64,
    .disp_gpio_num = ST7701S_DISP_EN_GPIO,  // tu peux garder à -1 si inutilisé
    .de_gpio_num = ST7701S_DE_GPIO,
    .vsync_gpio_num = ST7701S_VSYNC_GPIO,
    .hsync_gpio_num = ST7701S_HSYNC_GPIO,
    .pclk_gpio_num = ST7701S_PCLK_GPIO,
    .data_gpio_nums = {
        ST7701S_DATA0_GPIO, ST7701S_DATA1_GPIO, ST7701S_DATA2_GPIO, ST7701S_DATA3_GPIO,
        ST7701S_DATA4_GPIO, ST7701S_DATA5_GPIO, ST7701S_DATA6_GPIO, ST7701S_DATA7_GPIO,
        ST7701S_DATA8_GPIO, ST7701S_DATA9_GPIO, ST7701S_DATA10_GPIO, ST7701S_DATA11_GPIO,
        ST7701S_DATA12_GPIO, ST7701S_DATA13_GPIO, ST7701S_DATA14_GPIO, ST7701S_DATA15_GPIO
    },
    
    .flags = {
        .fb_in_psram = true,
    }
};
void display_draw_icon_rotated(int cx, int cy, const uint16_t* icon, float angle_deg);

void delay_ms(int ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void display_qibla_and_times(float heading, float bearing, float distance) {
    const image_t* bg = image_manager_get_background(2);
    display_draw_image(0, 0, bg->width, bg->height, bg);

    // 🕌 Affichage de l’angle Qibla et distance
    char qibla_line[64];
    snprintf(qibla_line, sizeof(qibla_line), "Qibla: %.0f° – %.0f km", fmodf(bearing - heading + 360.0f, 360.0f), distance);
    display_draw_text(20, 20, qibla_line, COLOR_CYAN);

    // 📿 Titre
    display_draw_text(20, 50, translate(STR_HORAIRES_PRIERE), COLOR_YELLOW);

    // 📿 Liste des 5 prières avec leurs horaires
    const char **times = prayer_get_all_times();
    for (int i = 0; i < 5; i++) {
        char line[64];
        snprintf(line, sizeof(line), "%s: %s", translate((lang_str_id_t)(STR_FAJR + i)), times[i]);
        display_draw_text(20, 80 + i * 25, line, COLOR_WHITE);
    }

    display_draw_status_bar();
}


//float qmi8658_get_heading() { return 120.0f; }  

const char* get_display_time() {
    if (gps_has_fix()) return gps_get_local_time();
    time_t now; struct tm t; time(&now); localtime_r(&now, &t);
    static char fallback[6]; snprintf(fallback, sizeof(fallback), "%02d:%02d", t.tm_hour, t.tm_min); return fallback;
}


void display_draw_status_bar() {
    display_draw_text(10, 5, get_display_time(), COLOR_WHITE);
    display_draw_icon(LCD_WIDTH - 40, 5, gps_has_fix() ? icon_gps_ok : icon_gps_waiting);
    display_draw_icon(LCD_WIDTH - 20, 5, battery_get_icon());
}

void display_home_screen() {
    const image_t* bg = image_manager_get_background(is_night_mode() ? 5 : 0);
    display_draw_image(0, 0, bg->width, bg->height, bg);
    display_draw_text(80, 60, get_display_time(), COLOR_WHITE);
    display_draw_text(80, 140, prayer_get_next_name(), COLOR_GREEN);
    if (battery_monitor_is_charging()) display_draw_icon(100, 200, icon_battery_charge);
}

void display_show_prayer_state() {
    int rakah = prayer_get_current_rakah(), sujood = prayer_get_current_sujood();
    const char *posture = prayer_get_current_posture();
    display_draw_text(80, 30, posture, COLOR_YELLOW);

    for (int i = 0; i < prayer_get_total_rakah(); i++)
        display_draw_icon(30 + i * 30, 100, (i < rakah) ? icon_rakah_dot_active : icon_rakah_dot_inactive);

    for (int j = 0; j < 2; j++)
        display_draw_icon(90 + j * 30, 150, (j < sujood) ? icon_sujood_current : icon_sujood_inactive);
}

void display_intro_animation() {
    const image_t* bg = image_manager_get_background(0);  // Mecca = index 0
    display_draw_image(0, 0, bg->width, bg->height, bg);
    display_draw_text(20, 100, "Bienvenue", COLOR_WHITE);
    delay_ms(1500);
}

void qibla_display_with_data(float bearing, float distance) {
    const image_t* bg = image_manager_get_background(2);
    display_draw_image(0, 0, bg->width, bg->height, bg);
    float heading = qmi8658_get_heading();
    float angle = fmodf(bearing - heading + 360, 360);

    display_draw_icon_rotated(110, 100, icon_qibla_arrow, angle);
    display_draw_text(60, 30, translate(STR_QIBLA_DIRECTION), COLOR_YELLOW);

    char dist_str[32];
    snprintf(dist_str, sizeof(dist_str), "%s: %.1f km", translate(STR_DISTANCE), distance);
    display_draw_text(60, 60, dist_str, COLOR_WHITE);
}

void hadith_display_today(int index, int lang) {
    const image_t* bg = image_manager_get_background(3);
    display_draw_image(0, 0, bg->width, bg->height, bg);
    display_draw_text(20, 60, translate(STR_HADITH_DU_JOUR), COLOR_YELLOW);
    display_draw_text(20, 90, (const char*)hadith_get_by_index(index, lang), COLOR_WHITE);
}

void prayer_display_times(float heading, float bearing) {
    const image_t* bg = image_manager_get_background(4);
    display_draw_image(0, 0, bg->width, bg->height, bg);

    display_draw_text(20, 20, translate(STR_HORAIRES_PRIERE), COLOR_YELLOW);

    const char **labels = prayer_get_all_names();  // ⚠ utilisé juste pour éviter le warning
    (void)labels; // si tu préfères garder la ligne sans l’utiliser

    const char **times = prayer_get_all_times();

    for (int i = 0; i < 5; i++) {
        char line[64];
        snprintf(line, sizeof(line), "%s: %s", translate(STR_FAJR + i), times[i]);
        display_draw_text(20, 50 + i * 30, line, COLOR_WHITE);
    }

    // 👉 Affichage de l’angle Qibla en haut à droite
    float angle = fmodf(bearing - heading + 360.0f, 360.0f);
    char angle_str[32];
    snprintf(angle_str, sizeof(angle_str), "Qibla: %.0f°", angle);
    display_draw_text(160, 20, angle_str, COLOR_CYAN);

    display_draw_status_bar();  // GPS, batterie, etc.
}

void display_render_menu(const char **menu_items, int selected) {
    const image_t* background = image_manager_get_background(1);
    display_draw_image(0, 0, background->width, background->height, background);
    for (int i = 0; i < 5; i++) {
        uint16_t color = (i == selected) ? COLOR_CYAN : COLOR_WHITE;
        display_draw_text(30, 30 + i * 30, translate((lang_str_id_t)(STR_MENU_THEME + i)), color);
    }
}

void display_menu_interactive(int selected, int btn_event) {
    switch (selected) {
        case 0: config_menu_open_theme(btn_event); break;
        case 1: config_menu_open_settings(btn_event); break;
        case 2: {
        float heading = 0.0f;
        float bearing = qibla_display_get_qibla_angle();
    prayer_display_times(heading, bearing);
    break;
}
        case 3: hadith_display_today(0, 0); break;
        case 4: break;
    }
}


void display_draw_icon(int x, int y, const uint16_t* icon) {
    int size = 16; // ou adapte si tes icônes sont 32x32, etc.

    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; i++) {
            uint16_t color = icon[j * size + i];
            if (color != 0x0000) { // optionnel : ignorer fond noir
                display_draw_pixel(x + i, y + j, color);
            }
        }
    }
}

void display_draw_icon_rotated(int cx, int cy, const uint16_t* icon, float angle_deg) {
    const int size = 16; // ⚠️ Change ça si ton icône est d'une autre taille
    float angle_rad = angle_deg * M_PI / 180.0f;

    float cos_a = cosf(angle_rad);
    float sin_a = sinf(angle_rad);

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int dx = x - size / 2;
            int dy = y - size / 2;

            int src_x = (int)( dx * cos_a + dy * sin_a + size / 2);
            int src_y = (int)(-dx * sin_a + dy * cos_a + size / 2);

            if (src_x >= 0 && src_x < size && src_y >= 0 && src_y < size) {
                uint16_t color = icon[src_y * size + src_x];
                if (color != 0x0000) { // optionnel : ignore fond noir
                    display_draw_pixel(cx + dx, cy + dy, color);
                }
            }
        }
    }
}


void display_draw_text(int x, int y, const char* text, uint16_t color) {
    // Implémentation de l'affichage de texte pixel par pixel
    while (*text) {
        draw_char_zoomed(x, y, *text, color);  // ou ton propre rendu
        x += 8;  // Avance de 8px (si police 8x8)
        text++;
    }
}

void display_draw_image(int x, int y, int w, int h, const image_t* img) {
    for (int iy = 0; iy < h; iy++) {
        for (int ix = 0; ix < w; ix++) {
            uint16_t pixel = img->data[iy * w + ix];
            if (pixel != 0x0000) { // ignore noir
                display_draw_pixel(x + ix, y + iy, pixel);
            }
        }
    }
}

void display_draw_pixel(int x, int y, uint16_t color) {
    uint16_t pixel_data[1] = { color };
    if (lcd_panel_handle && lcd_panel_handle->ops && lcd_panel_handle->ops->draw_bitmap) {
        lcd_panel_handle->ops->draw_bitmap(lcd_panel_handle, x, y, x + 1, y + 1, pixel_data);
    }
}

void st7701s_display_fade_intro(void) {
    // Animation intro
    }

void st7701s_display_show_boot_info(void) {
    // Texte ou logo de démarrage
    }

void st7701s_display_draw_icon(int x, int y, const uint16_t *icon_data) {
    if (!icon_data || !lcd_panel_handle) return;

    // Affiche une icône de 16x16 (ou adapte si besoin)
    esp_lcd_panel_draw_bitmap(
        lcd_panel_handle,
        x, y,
        x + ICON_WIDTH, y + ICON_HEIGHT,
        (void *)icon_data
    );
}

void display_draw_char(int x, int y, char c, uint16_t color, int font_size) {
    const uint8_t* glyph = font8x8_basic[(uint8_t)c];
    for (int row = 0; row < 8; row++) {
        uint8_t line = glyph[row];
        for (int col = 0; col < 8; col++) {
            if (line & (1 << col)) {
                // dessine un pixel ou un bloc (selon taille police)
                for (int dy = 0; dy < font_size; dy++) {
                    for (int dx = 0; dx < font_size; dx++) {
                        st7701s_draw_pixel(x + col * font_size + dx, y + row * font_size + dy, color);
                    }
                }
            }
        }
    }
}

    
void st7701s_display_draw_text_centered(const char* text, int y, uint16_t color) {
    int font_size = 1;
    int char_width = 8 * font_size;
    int len = strlen(text);
    int total_width = len * char_width;
    int start_x = (240 - total_width) / 2;

    for (int i = 0; i < len; i++) {
        char c = text[i];
        display_draw_char(start_x + i * char_width, y, c, color, font_size);
    }
}


void st7701s_display_draw_battery(int level_percent, bool is_charging) {
    const uint16_t *icon = NULL;

    if (is_charging) {
        icon = icon_battery_charging;
    } else if (level_percent > 80) {
        icon = icon_battery_100;
    } else if (level_percent > 60) {
        icon = icon_battery_75;
    } else if (level_percent > 40) {
        icon = icon_battery_50;
    } else if (level_percent > 20) {
        icon = icon_battery_25;
    } else {
        icon = icon_battery_low;
    }

    st7701s_display_draw_icon(210, 5, icon);  
}

void st7701s_display_draw_text_multiline(const char* text, int y, uint16_t color) {
    int font_size = 1;
    int char_width = 8 * font_size;
    int char_height = 8 * font_size;
    int x = 10;
    int max_width = 220;

    for (int i = 0; text[i] != '\0'; ++i) {
        if (text[i] == '\n' || (x + char_width > max_width)) {
            y += char_height + 2;
            x = 10;
            if (text[i] == '\n') continue;
        }
        display_draw_char(x, y, text[i], color, font_size);
        x += char_width;
    }
}

void st7701s_display_clear(void) {
    // uint16_t color = 0x0000; // Couleur noire
    // esp_lcd_panel_draw_bitmap(lcd_panel_handle, 0, 0, LCD_WIDTH, LCD_HEIGHT, &color);

    // the above function is a placeholder, you may need to implement the actual clear function
}

void st7701s_display_init(void){
    esp_err_t ret = esp_lcd_new_rgb_panel(&panel_config, &lcd_panel_handle);
  //  if (ret != ESP_OK) {
  //      ESP_LOGE("ST7701S", "Failed to initialize LCD panel: %s", esp_err_to_name(ret));
  //      return;
  //  }

    // theabove function is a placeholder, you may need to implement the actual init function
}

void st7701s_display_distance_status(uint8_t status) {
    // const uint16_t* icon = NULL;

    // switch (status) {
    //     case 0: icon = icon_sujood; break; // OK
    //     case 1: icon = icon_sujood_1; break; // Warning
    //     case 2: icon = icon_sujood_2; break; // Error
    //     default: return;
    // }

    // st7701s_display_draw_icon(10, 10, icon);
    // theabove function is a placeholder, you may need to implement the actual init function
}

void st7701s_draw_pixel(int x, int y, uint16_t color)
{
     if (lcd_panel_handle && lcd_panel_handle->ops && lcd_panel_handle->ops->draw_bitmap) {
        lcd_panel_handle->ops->draw_bitmap(lcd_panel_handle, x, y, x + 1, y + 1, &color);
     }
    // theabove function is a placeholder, you may need to implement the actual init function
} 





#ifdef __cplusplus
}
#endif




