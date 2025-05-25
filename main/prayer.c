// prayer.c

#include "prayer.h"
#include "lang.h"
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif


// 🕌 Liste des noms de prières (traduits dynamiquement)
const char* prayer_get_next_name() {
    // Par défaut, retourne "Asr" (à adapter selon l'heure réelle plus tard)
    return translate(STR_ASR);
}

int prayer_get_current_rakah() {
    // Exemple simple : retourne 2 rak'ahs par défaut
    return 2;
}

int prayer_get_total_rakah() {
    return 4;
}

int prayer_get_current_sujood() {
    return 1;
}

const char* prayer_get_current_posture() {
    return "Sujood";
}

const char** prayer_get_all_names() {
    static const char* names[5];
    names[0] = translate(STR_FAJR);
    names[1] = translate(STR_DHUHR);
    names[2] = translate(STR_ASR);
    names[3] = translate(STR_MAGHRIB);
    names[4] = translate(STR_ISHA);
    return names;
}

const char** prayer_get_all_times() {
    static const char* times[] = {
        "05:12", "13:10", "16:40", "18:55", "20:20"
    };
    return times;
}

#ifdef __cplusplus
}
#endif