#define ESP_LCD_RGB_INCLUDE_DEFAULT_PANEL_OPS  // ✅ Doit venir AVANT tout

#include "esp_lcd_panel_ops.h"                // Doit être après la define
#include "st7701s.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_interface.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <stdlib.h>
#include "soc/lcd_periph.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_io.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_lcd_types.h"

#ifdef __cplusplus
extern "C" {
#endif



esp_err_t esp_lcd_panel_reset(esp_lcd_panel_handle_t panel);
esp_err_t esp_lcd_panel_init(esp_lcd_panel_handle_t panel);
esp_err_t esp_lcd_panel_disp_on_off(esp_lcd_panel_handle_t panel, bool on_off);

esp_lcd_panel_handle_t lcd_panel_handle = NULL;


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



void st7701s_init_panel()
{
    // 🔧 Configuration du GPIO RESET
    gpio_config_t rst_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << ST7701S_RST_GPIO,
    };
    gpio_config(&rst_config);

    // 🔄 Reset matériel de l’écran
    gpio_set_level(ST7701S_RST_GPIO, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(ST7701S_RST_GPIO, 1);
    vTaskDelay(pdMS_TO_TICKS(10));

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

// ✅ Initialisation de l'écran avec ESP-IDF
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &lcd_panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(lcd_panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(lcd_panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd_panel_handle, true));
}

#ifdef __cplusplus
}
#endif

