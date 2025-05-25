// main.cpp — version adaptée à l’ensemble du projet sans modifier les noms des fichiers existants
#include <stdio.h>              // Standard C
#include <string>              // ✅ Nécessaire pour std::string

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "nvs_flash.h"

#ifdef __cplusplus
extern "C" {
#endif



#include "st7701s_display.h"
#include "gps_parser.h"

#include "font8x8_basic.h"
#include "image_manager.h"
#include "icons.h"
#include "lang.h"
#include "config_menu.h"
#include "espnow_sync.h"

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

#include "input_manager.h"  // ou tout autre header C

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif



#include "battery_monitor.h"
#include "prayer.h"
#include "hadiths.h"
#include "qibla_display.h"

#include "buzzer.h"

#include "color.h"  // ✅ Pour les définitions COLOR_WHITE, COLOR_CYAN, etc.



#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

#include "vl53l0x_sensor.h"
#include "vl53l0x_platform.h"

#ifdef __cplusplus
}
#endif


#define TAG "MAIN"

// Configuration boutons GPIO
#define BUTTON_MENU GPIO_NUM_0
#define BUTTON_ACTION GPIO_NUM_3

static int rakah_count = 0;
static int sujood_count = 0;
static int prayer_type = 2; // Exemple : 2 rak'ahs (modifié via GPS ou config plus tard)


extern "C" void display_qibla_and_times(float heading, float bearing, float distance);

void update_display() {
    st7701s_display_clear();

    // Affiche l’heure et position GPS
    if (gps_has_fix()) {
    st7701s_display_draw_text_centered(gps_get_local_time(), 40, COLOR_WHITE);

    std::string city = gps_get_location_name();
    if (city == "World" || city == "No Fix") {
        city = "🌍 Unknown Area";
    }

    std::string location = "📍 " + city;
    st7701s_display_draw_text_centered(location.c_str(), 70, COLOR_CYAN);
    }
    else {
    st7701s_display_draw_text_centered("--:--", 40, COLOR_GRAY);
    st7701s_display_draw_text_centered("GPS...", 70, COLOR_ORANGE);
    }



  
    // Affiche le nom de la prochaine prière
    const char* prayer_name = prayer_get_next_name();
    st7701s_display_draw_text_centered(prayer_name, 110, COLOR_YELLOW);

    // Affiche les icônes de rak'ah et sujood
    for (int i = 0; i < prayer_type; ++i) {
        const uint16_t* icon = (i < rakah_count) ? icon_rakah_dot_active : icon_rakah_dot_inactive;
        st7701s_display_draw_icon(60 + i * 40, 160, icon);
    }

    if (sujood_count == 1)
        st7701s_display_draw_icon(180, 200, icon_sujood_1);
    else if (sujood_count == 2)
        st7701s_display_draw_icon(180, 200, icon_sujood_2);

    // Batterie
    st7701s_display_draw_battery(battery_get_level_percent(), battery_monitor_is_charging()
    );

    // Hadith du jour
    const char* hadith = hadiths_get_today();
    st7701s_display_draw_text_multiline(hadith, 240, COLOR_WHITE);

      // Qibla (affichée seulement si GPS actif)
    if (gps_has_fix()) {
    float heading = 0.0f;  // ou imu_get_heading() plus tard
    float bearing = qibla_display_get_qibla_angle();
    float distance = gps_get_qibla_distance();

    display_qibla_and_times(heading, bearing, distance);
    }
  
}

extern "C" void app_main() {
    nvs_flash_init();
    lang_init();
    config_menu_init();

    st7701s_display_init();
    buzzer_init(GPIO_NUM_2);  // ✅ Ajoute cette ligne ici

    i2c_master_init();
    vl53l0x_init();
    
    gps_parser_init(UART_NUM_1);
    battery_monitor_init((adc_channel_t)ADC1_CHANNEL_6, GPIO_NUM_6);
    espnow_sync_init();
    input_manager_init();

    st7701s_display_fade_intro();
    st7701s_display_show_boot_info();

    gpio_set_direction(BUTTON_MENU, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_MENU, GPIO_PULLUP_ONLY);
    gpio_set_direction(BUTTON_ACTION, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_ACTION, GPIO_PULLUP_ONLY);

    while (1) {
        // Détection sujood
        // Détection sujood
        if (vl53l0x_sujood_detected()) {
        sujood_count++;
        buzzer_beep_sujood();

        if (sujood_count == 2) {
        sujood_count = 0;
        rakah_count++;
        if (rakah_count >= prayer_type) {
            rakah_count = 0;
            buzzer_beep_end();
        }
    }
}

    // ✅ Affiche le statut de la mesure VL53L0X
    st7701s_display_distance_status(vl53l0x_get_last_status());

    update_display();
    vTaskDelay(pdMS_TO_TICKS(1000));
}

         // 🔁 Gestion complète des boutons
         
         
    button_event_t btn = input_manager_get_event();

    switch (btn) {
        case BUTTON_SELECT:
        config_menu_show();         // Affiche le menu normal
        update_display();
        break;

        case BUTTON_SELECT_LONG:
        config_menu_open_settings(0);  // Ouvre directement les réglages
        update_display();
        break;

        case BUTTON_UP:
        // Action perso (ex: revenir en arrière ?)
        break;

        case BUTTON_DOWN:
        // Action perso (ex: afficher prochaine prière ?)
        break;

    default:
        break;
    }

        // Affichage périodique
        update_display();
        vTaskDelay(pdMS_TO_TICKS(3000));
}

