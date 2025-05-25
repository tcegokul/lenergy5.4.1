// config_menu.c
#include "esp_log.h"
#include "st7701s_display.h"
#include "config_menu.h"
#include "color.h"

#include "nvs_flash.h"
#include "nvs.h"
#include "image_manager.h"
#include "lang.h"
#include "icons.h"
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


void display_main_menu_translated(void);

static config_t current_config;
static bool config_saved_message = false;

void config_reset_to_default() {
    current_config.language = LANG_EN;
    current_config.buzzer_volume = 50;
    current_config.dark_theme = false;
    current_config.battery_alert_threshold = 10;
    current_config.night_mode_start = 22;
    current_config.night_mode_end = 5;
    current_config.background_id = 0;
    current_config.sound_type = SOUND_SIMPLE;
    current_config.qibla_manual_offset = 0;
    current_config.sleep_timeout_seconds = 60;
}

void config_menu_init() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    nvs_handle_t nvs;
    size_t required_size = sizeof(config_t);
    if (nvs_open("menu", NVS_READONLY, &nvs) == ESP_OK) {
        if (nvs_get_blob(nvs, "config", &current_config, &required_size) != ESP_OK) {
            config_reset_to_default();
        }
        nvs_close(nvs);
    } else {
        config_reset_to_default();
    }
}

bool config_save_settings() {
    nvs_handle_t nvs;
    if (nvs_open("menu", NVS_READWRITE, &nvs) == ESP_OK) {
        nvs_set_blob(nvs, "config", &current_config, sizeof(config_t));
        nvs_commit(nvs);
        nvs_close(nvs);
        config_saved_message = true;
        return true;
    }
    return false;
}

config_t config_get() {
    return current_config;
}

void config_set(config_t new_config) {
    current_config = new_config;
    config_save_settings();
}

void config_menu_show() {
    // Menu intelligent : suggestions selon l’heure ou disponibilité GPS
    // Aperçu en direct : appeler draw_preview_theme() ou draw_preview_background() à chaque sélection
}

void config_menu_open_theme(int btn_event) {
    display_draw_image(0, 0, 240, 240, image_manager_get_background(1));
    display_draw_text(30, 30, translate(STR_CHOOSE_THEME), COLOR_CYAN);
    display_draw_text(30, 70, translate(STR_THEME_DESC), COLOR_WHITE);
    display_draw_status_bar();

    if (btn_event == 3) {
        display_main_menu_translated();
    }
}

#define SETTINGS_OPTION_COUNT 7
static int settings_cursor = 0;

const char* get_setting_label(int index) {
    static char buf[32];

    switch (index) {
        case 0:
            switch (current_config.language) {
                case LANG_EN: return "Langue: Anglais";
                case LANG_FR: return "Langue: Français";
                case LANG_AR: return "Langue: Arabe";
                case LANG_IT: return "Langue: Italien";
                default: return "Langue: Inconnue";
            }

        case 1:
            snprintf(buf, sizeof(buf), "Volume buzzer: %d%%", current_config.buzzer_volume);
            return buf;

        case 2:
            return current_config.dark_theme ? "Mode sombre: Activé" : "Mode sombre: Auto";

        case 3:
            snprintf(buf, sizeof(buf), "Alerte batterie: %d%%", current_config.battery_alert_threshold);
            return buf;

        case 4:
            switch (current_config.sound_type) {
                case SOUND_SIMPLE: return "Son: Simple";
                case SOUND_SPIRITUAL: return "Son: Spirituel";
                case SOUND_SILENT: return "Son: Muet";
                default: return "Son: Inconnu";
            }

        case 5:
            snprintf(buf, sizeof(buf), "Fond d'ecran: %d", current_config.background_id);
            return buf;

        case 6:
            snprintf(buf, sizeof(buf), "Veille auto: %ds", current_config.sleep_timeout_seconds);
            return buf;

        default:
            return "";
    }
}


void draw_icon_for_setting(int setting_index, int x, int y) {
    switch (setting_index) {
        case 0: display_draw_icon(x, y, icon_language); break;
        case 1: display_draw_icon(x, y, icon_buzzer); break;
        case 2: display_draw_icon(x, y, current_config.dark_theme ? icon_night_mode : icon_day_mode); break;
        case 3: display_draw_icon(x, y, icon_battery_25); break;
        case 4:
            if (current_config.sound_type == SOUND_SIMPLE) display_draw_icon(x, y, icon_check);
            else if (current_config.sound_type == SOUND_SPIRITUAL) display_draw_icon(x, y, icon_hadith);
            else display_draw_icon(x, y, icon_sujood_done);
            break;
        case 5: display_draw_icon(x, y, icon_theme); break;
        case 6: display_draw_icon(x, y, icon_clock); break;
        default: break;
    }
}

void config_menu_open_settings(int btn_event) {
    if (btn_event == 0) settings_cursor = (settings_cursor - 1 + SETTINGS_OPTION_COUNT) % SETTINGS_OPTION_COUNT;
    if (btn_event == 1) settings_cursor = (settings_cursor + 1) % SETTINGS_OPTION_COUNT;

    if (btn_event == 2) {
        switch (settings_cursor) {
            case 0: current_config.language = (current_config.language + 1) % 4; break;
            case 1: current_config.buzzer_volume = (current_config.buzzer_volume + 10) % 110; break;
            case 2: current_config.dark_theme = !current_config.dark_theme; break;
            case 3:
                current_config.battery_alert_threshold += 5;
                if (current_config.battery_alert_threshold > 30)
                    current_config.battery_alert_threshold = 5;
                break;
            case 4: current_config.sound_type = (current_config.sound_type + 1) % 3; break;
            case 5: current_config.background_id = (current_config.background_id + 1) % image_manager_get_total(); break;
            case 6:
                if (current_config.sleep_timeout_seconds == 15)
                    current_config.sleep_timeout_seconds = 30;
                else if (current_config.sleep_timeout_seconds == 30)
                    current_config.sleep_timeout_seconds = 60;
                else if (current_config.sleep_timeout_seconds == 60)
                    current_config.sleep_timeout_seconds = 120;
                else
                    current_config.sleep_timeout_seconds = 15;
                break;
        }
        config_save_settings();
    }

    display_draw_image(0, 0, 240, 240, image_manager_get_background(current_config.background_id));

    for (int i = 0; i < 3; i++) {
        int item = (settings_cursor + i) % SETTINGS_OPTION_COUNT;
        display_draw_text(20, 30 + i * 40, get_setting_label(item), i == 0 ? COLOR_YELLOW : COLOR_WHITE);
        if (i == 0) draw_icon_for_setting(item, 200, 30 + i * 40);
    }

    if (config_saved_message) {
        display_draw_text(20, 160, "✔ Sauvegardé", COLOR_GREEN);
        config_saved_message = false;
    }

    display_draw_status_bar();

    if (btn_event == 3) {
        display_main_menu_translated();
    }
}

void display_main_menu_translated() {
    const char* menu_items[5];
    menu_items[0] = translate(STR_MENU_THEME);
    menu_items[1] = translate(STR_SETTINGS);
    menu_items[2] = translate(STR_PRAYER_TIMES);
    menu_items[3] = translate(STR_HADITH_TODAY);
    menu_items[4] = translate(STR_QIBLA_DIRECTION);

    display_render_menu(menu_items, 0);
}

#ifdef __cplusplus
}
#endif