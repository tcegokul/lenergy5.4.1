#ifndef ESP_LCD_PANEL_VENDOR_H
#define ESP_LCD_PANEL_VENDOR_H

#include "esp_err.h"
#include "esp_lcd_panel_interface.h"  // Nécessaire pour les types panel_io et panel_handle

#ifdef __cplusplus
extern "C" {
#endif

// Cette fonction initialise le panneau ST7701S
esp_err_t esp_lcd_new_panel_st7701s(const esp_lcd_panel_io_handle_t panel_io,
                                    esp_lcd_panel_handle_t *ret_panel);

#ifdef __cplusplus
}
#endif

#endif // ESP_LCD_PANEL_VENDOR_H
