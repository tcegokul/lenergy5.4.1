#include "esp_lcd_types.h"
#include "esp_lcd_panel_rgb.h"  // ✅ Fournit déjà la struct officielle

#ifndef ESP_LCD_PANEL_INTERFACE_H
#define ESP_LCD_PANEL_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct esp_lcd_panel_io_t *esp_lcd_panel_io_handle_t;
typedef struct esp_lcd_panel_t *esp_lcd_panel_handle_t;

/**
 * @brief Fonction d'initialisation du panneau LCD RGB
 */
esp_err_t esp_lcd_new_rgb_panel(const esp_lcd_rgb_panel_config_t *panel_config,
                                esp_lcd_panel_handle_t *ret_panel);

#ifdef __cplusplus
}
#endif

#endif // ESP_LCD_PANEL_INTERFACE_H
