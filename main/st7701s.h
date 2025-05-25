#pragma once

#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "driver/spi_master.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LCD_HOST       SPI2_HOST
#define LCD_PIXEL_CLOCK_HZ     (20 * 1000 * 1000)
#define LCD_BK_LIGHT_ON_LEVEL  1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL

#define PIN_NUM_MOSI  11
#define PIN_NUM_CLK   12
#define PIN_NUM_CS    10
#define PIN_NUM_DC    13
#define PIN_NUM_RST   14
#define PIN_NUM_BK    21

#define LCD_H_RES     480
#define LCD_V_RES     480

esp_err_t st7701s_lcd_init(esp_lcd_panel_handle_t *ret_panel);

#ifdef __cplusplus
}
#endif
