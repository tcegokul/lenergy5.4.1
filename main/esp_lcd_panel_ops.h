#ifndef ESP_LCD_PANEL_OPS_H
#define ESP_LCD_PANEL_OPS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>   // ✅ Ajoute cette ligne pour que 'bool' soit reconnu
#include "esp_err.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Handle abstrait pour un panneau LCD
 */
typedef struct esp_lcd_panel_t *esp_lcd_panel_handle_t;

/**
 * @brief Structure définissant les opérations possibles sur un panneau LCD
 */
typedef struct {
    /**
     * @brief Dessine une image bitmap à l’écran.
     * 
     * @param panel Le handle du panneau
     * @param x_start Coordonnée X de début
     * @param y_start Coordonnée Y de début
     * @param x_end Coordonnée X de fin
     * @param y_end Coordonnée Y de fin
     * @param color_data Données de couleur (RGB565 ou RGB888)
     * 
     * @return ESP_OK ou code d'erreur
     */
    esp_err_t (*draw_bitmap)(esp_lcd_panel_handle_t panel, int x_start, int y_start, int x_end, int y_end, const void *color_data);

    /**
     * @brief Active ou désactive le rétroéclairage
     */
    esp_err_t (*set_backlight)(esp_lcd_panel_handle_t panel, bool on_off);

    /**
     * @brief Met le panneau en veille (écran noir)
     */
    esp_err_t (*sleep)(esp_lcd_panel_handle_t panel, bool enable);

    /**
     * @brief Désactive / réinitialise complètement le panneau
     */
    esp_err_t (*del)(esp_lcd_panel_handle_t panel);
} esp_lcd_panel_ops_t;

/**
 * @brief Structure de panneau LCD avec les opérations associées
 */
typedef struct esp_lcd_panel_t {
    void *user_ctx;                      ///< Contexte utilisateur (driver, config, etc.)
    const esp_lcd_panel_ops_t *ops;      ///< Opérations implémentées par le panneau
} esp_lcd_panel_t;

#ifdef __cplusplus
}
#endif

#endif // ESP_LCD_PANEL_OPS_H
