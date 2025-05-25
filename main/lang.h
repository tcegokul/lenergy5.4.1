#ifndef LANG_H
#define LANG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Définition des langues disponibles
typedef enum {
    LANG_EN = 0,
    LANG_FR,
    LANG_AR,
    LANG_IT,
    LANG_COUNT
} language_t;

// Identifiants pour toutes les chaînes traduisibles
typedef enum {
    STR_MENU,
    STR_SETTINGS,
    STR_LANGUAGE,
    STR_BATTERY,
    STR_TIME,
    STR_LOCATION,
    STR_PRAYER,
    STR_QIBLA,
    STR_HADITH,
    STR_NEXT_PRAYER,
    STR_CURRENT_POSTURE,
    STR_TOTAL_RAKAH,
    STR_DISTANCE,
    STR_HADITH_TODAY,
    STR_PRAYER_TIMES,
    STR_CHOOSE_THEME,
    STR_THEME_DESC,

 // ✅ Nouveaux identifiants
    STR_MENU_THEME,
    STR_CHOISIR_THEME,
    STR_LISTE_THEMES,
    STR_THEME_DAY,
    STR_THEME_NIGHT,
    STR_THEME_MECCA,
    STR_THEME_QIBLA,
    STR_QIBLA_DIRECTION,
    STR_HORAIRES_PRIERE,
    STR_HADITH_DU_JOUR, 


    // ✅ 🔥 Ajoute ceci pour la boucle STR_FAJR + i
    STR_FAJR,
    STR_DHUHR,
    STR_ASR,
    STR_MAGHRIB,
    STR_ISHA,

   
    STR_COUNT  // Doit toujours être le dernier
} lang_str_id_t;

// Initialise les traductions
void lang_init(void);

// Définit la langue active
void lang_set(language_t lang);

// Retourne la langue active
language_t lang_get(void);

// Traduit un identifiant de texte en chaîne localisée
const char* translate(lang_str_id_t phrase);

#ifdef __cplusplus
}
#endif

#endif // LANG_H
