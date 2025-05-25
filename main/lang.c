// lang.c
#include "lang.h"
#include "config_menu.h"
#include "gps_parser.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif



static int selected_lang = -1;

static const char* lang_table[STR_COUNT][LANG_COUNT] = {
    [STR_MENU]             = {"Menu", "القائمة", "Menu", "Menù"},
    [STR_SETTINGS]         = {"Settings", "الإعدادات", "Paramètres", "Impostazioni"},
    [STR_LANGUAGE]         = {"Language", "اللغة", "Langue", "Lingua"},
    [STR_BATTERY]          = {"Battery", "البطارية", "Batterie", "Batteria"},
    [STR_TIME]             = {"Time", "الوقت", "Heure", "Ora"},
    [STR_LOCATION]         = {"Location", "الموقع", "Localisation", "Posizione"},
    [STR_PRAYER]           = {"Prayer", "الصلاة", "Prière", "Preghiera"},
    [STR_QIBLA]            = {"Qibla", "القبلة", "Qibla", "Qibla"},
    [STR_HADITH]           = {"Hadith", "حديث", "Hadith", "Hadith"},
    [STR_NEXT_PRAYER]      = {"Next prayer", "الصلاة التالية", "Prière suivante", "Prossima preghiera"},
    [STR_CURRENT_POSTURE]  = {"Current posture", "الوضع الحالي", "Posture actuelle", "Posizione attuale"},
    [STR_TOTAL_RAKAH]      = {"Total rak'ahs", "عدد الركعات", "Nombre de rak‘ahs", "Rak‘ah totali"},
    [STR_DISTANCE]         = {"Distance", "المسافة", "Distance", "Distanza"},
    [STR_HADITH_TODAY]     = {"Hadith of the day", "حديث اليوم", "Hadith du jour", "Hadith del giorno"},
    [STR_PRAYER_TIMES]     = {"Prayer times", "أوقات الصلاة", "Horaires de prière", "Orari di preghiera"},
    [STR_CHOOSE_THEME]     = {"Choose a theme", "اختر موضوعًا", "Choisir un thème", "Scegli un tema"},
    [STR_THEME_DESC]       = {"Day / Night / Mecca / ...", "نهار / ليل / مكة / ...", "Jour / Nuit / Mecca / ...", "Giorno / Notte / Mecca / ..."},
    
    // ✅ Nouveaux ajouts
    [STR_MENU_THEME]       = {"Theme", "السمة", "Thème", "Tema"},
    [STR_CHOISIR_THEME]    = {"Choose a theme", "اختر سمة", "Choisir un thème", "Scegli un tema"},
    [STR_LISTE_THEMES]     = {"Theme list", "قائمة السمات", "Liste des thèmes", "Lista temi"},
    [STR_THEME_DAY]        = {"Day", "نهار", "Jour", "Giorno"},
    [STR_THEME_NIGHT]      = {"Night", "ليل", "Nuit", "Notte"},
    [STR_THEME_MECCA]      = {"Mecca", "مكة", "La Mecque", "La Mecca"},
    [STR_THEME_QIBLA]      = {"Qibla Style", "نمط القبلة", "Style Qibla", "Stile Qibla"},
    [STR_QIBLA_DIRECTION] = {"Qibla Direction", "اتجاه القبلة", "Direction de la Qibla", "Direzione Qibla"},
    [STR_HORAIRES_PRIERE] = {"Prayer Times", "أوقات الصلاة", "Horaires de prière", "Orari di preghiera"},
    [STR_HADITH_DU_JOUR] = {"Hadith of the Day", "حديث اليوم", "Hadith du jour", "Hadith del giorno"},

// ✅ Horaires de prière (nouveaux identifiants pour STR_FAJR + i)
    [STR_FAJR]            = {"Fajr", "الفجر", "Fajr", "Fajr"},
    [STR_DHUHR]           = {"Dhuhr", "الظهر", "Dohr", "Dhuhr"},
    [STR_ASR]             = {"Asr", "العصر", "Asr", "Asr"},
    [STR_MAGHRIB]         = {"Maghrib", "المغرب", "Maghreb", "Maghreb"},
    [STR_ISHA]            = {"Isha", "العشاء", "Icha", "Isha"}
    
};


static int detect_lang_from_gps() {
    float lat = gps_get_latitude();
    float lon = gps_get_longitude();

    if (lat == 0.0f && lon == 0.0f) return LANG_EN;  // pas de fix → anglais

    // France / Belgique / Suisse romande
    if (lat > 35 && lat < 49 && lon > -5 && lon < 10) return LANG_FR;

    // Italie
    if (lat > 36 && lat < 47 && lon > 6 && lon < 19) return LANG_IT;

    // Pays arabes connus (zones approximatives)
    if (
        // Maghreb
        (lat >= 19 && lat <= 37 && lon >= -17 && lon <= 11) || // Maroc, Algérie, Tunisie, Libye, Mauritanie
        // Moyen-Orient
        (lat >= 22 && lat <= 39 && lon >= 32 && lon <= 60) ||  // Égypte, Arabie Saoudite, Yémen, Oman, EAU, Qatar, Koweït
        (lat >= 29 && lat <= 37 && lon >= 35 && lon <= 42) ||  // Jordanie, Syrie, Palestine
        // Soudan & Irak
        (lat >= 12 && lat <= 23 && lon >= 22 && lon <= 37) ||  // Soudan
        (lat >= 29 && lat <= 37 && lon >= 43 && lon <= 49)     // Irak
    ) return LANG_AR;

    return LANG_EN;
}

void lang_init() {
    config_t cfg = config_get();
    if (cfg.language < 0 || cfg.language > 3) {
        cfg.language = detect_lang_from_gps();
        config_set(cfg); // on sauvegarde la langue détectée une fois
    }
    selected_lang = cfg.language;
}

int get_selected_language() {
    return selected_lang;
}

const char* translate(lang_str_id_t phrase_id) {
    if (phrase_id >= 0 && phrase_id < STR_COUNT) {
        return lang_table[phrase_id][selected_lang];
    }
    return "[?]";
}

#ifdef __cplusplus
}
#endif