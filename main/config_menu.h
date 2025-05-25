#ifndef CONFIG_MENU_H
#define CONFIG_MENU_H

#include <stdbool.h>
#include "lang.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SOUND_SIMPLE,
    SOUND_SPIRITUAL,
    SOUND_SILENT
} sound_type_t;

typedef struct {
    int language;                    // LANG_EN, LANG_AR, etc.
    int buzzer_volume;              // 0 à 100
    bool dark_theme;                // Mode sombre activé ?
    int battery_alert_threshold;    // % de batterie pour alerte
    int night_mode_start;           // Heure début mode nuit (ex: 22)
    int night_mode_end;             // Heure fin mode nuit (ex: 5)
    int background_id;              // ID du fond sélectionné
    sound_type_t sound_type;        // Type de son
    int qibla_manual_offset;        // Correction manuelle Qibla
    int sleep_timeout_seconds;      // Inactivité avant veille
} config_t;

// Fonctions principales
void config_menu_init();
void config_reset_to_default();
bool config_save_settings();
config_t config_get();
void config_set(config_t new_config);

// Fonctions d'affichage interactif
void config_menu_open_theme(int btn_event);
void config_menu_open_settings(int btn_event);
void config_menu_show();

#ifdef __cplusplus
}
#endif

#endif // CONFIG_MENU_H

