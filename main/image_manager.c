#include "color.h"
#include "image_manager.h"
#include "config_menu.h"

#include "bg_1.h"
#include "bg_2.h"
#include "bg_3.h"
#include "bg_1_data_night.h"
#include "bg_2_data_night.h"
#include "bg_3_data_night.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


// Fonction globale pour le mode nuit (liée à la config)
bool is_night_mode() {
    return config_get().dark_theme;  // champ existant dans config_t
}

static const image_t backgrounds[] = {
    { bg_1_data, bg_1_data_night, 240, 240 },
    { bg_2_data, bg_2_data_night, 240, 240 },
    { bg_3_data, bg_3_data_night, 240, 240 }
};

#define TOTAL_BACKGROUNDS (sizeof(backgrounds) / sizeof(backgrounds[0]))

void image_manager_init() {
    // Optionnel : chargement ou initialisation future
}

const image_t* image_manager_get_background(int index) {
    if (index < 0 || index >= TOTAL_BACKGROUNDS) index = 0;
    return &backgrounds[index];
}

const image_t* image_manager_get_preview(int index) {
    // Ici on retourne la même image, mais on pourrait créer des miniatures séparées
    return image_manager_get_background(index);
}

int image_manager_get_total() {
    return TOTAL_BACKGROUNDS;
}

#ifdef __cplusplus
}
#endif