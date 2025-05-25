// hadiths.c

#include <time.h>
#include "hadiths.h"
#include "config_menu.h"

#ifdef __cplusplus
extern "C" {
#endif

// === Français ===
static const char* hadiths_fr[] = {
    "Les actes ne valent que par les intentions.",
    "Le meilleur des hommes est le plus utile aux autres.",
    "Le Prophète était plus généreux que le vent.",
    "Un sourire est une aumône.",
    "Dieu aime ceux qui se repentent.",
    "Le paradis est sous les pieds des mères.",
    "Celui qui croit en Dieu dit du bien ou se tait.",
    "La foi est composée de 70 branches.",
    "Aucun d’entre vous ne croit vraiment tant qu’il n’aime pas pour son frère ce qu’il aime pour lui-même.",
    "L’envie ronge les bonnes actions comme le feu ronge le bois.",
    "Celui qui ne remercie pas les gens, ne remercie pas Dieu.",
    "La patience est lumière.",
    "L’Islam est basé sur cinq piliers.",
    "Celui qui pratique la pudeur est aimé d’Allah.",
    "Demande à Allah même pour le lacet de ta chaussure.",
    "Celui qui nous trompe n’est pas des nôtres.",
    "Dieu ne regarde pas vos corps mais vos cœurs.",
    "Dieu est avec les patients.",
    "L’honnêteté mène au Paradis.",
    "La miséricorde ne se retire que des cœurs durs."
};

// === English ===
static const char* hadiths_en[] = {
    "Actions are judged by intentions.",
    "The best among you is the most beneficial to others.",
    "The Prophet was more generous than the blowing wind.",
    "A smile is charity.",
    "Allah loves those who repent.",
    "Paradise lies at the feet of mothers.",
    "Speak good or remain silent.",
    "Faith consists of 70 branches.",
    "Love for your brother what you love for yourself.",
    "Envy eats good deeds like fire eats wood.",
    "He who doesn’t thank people doesn’t thank Allah.",
    "Patience is light.",
    "Islam is built on five pillars.",
    "Modesty brings Allah’s love.",
    "Ask Allah even for your shoelace.",
    "He who cheats is not one of us.",
    "Allah looks at hearts, not bodies.",
    "Allah is with the patient.",
    "Truthfulness leads to Paradise.",
    "Mercy is removed only from the cruel-hearted."
};

// === Arabe ===
static const char* hadiths_ar[] = {
    "إنما الأعمال بالنيات.",
    "خير الناس أنفعهم للناس.",
    "كان النبي أجود من الريح المرسلة.",
    "تبسمك في وجه أخيك صدقة.",
    "إن الله يحب التوابين.",
    "الجنة تحت أقدام الأمهات.",
    "من كان يؤمن بالله فليقل خيرًا أو ليصمت.",
    "الإيمان بضع وسبعون شعبة.",
    "لا يؤمن أحدكم حتى يحب لأخيه ما يحب لنفسه.",
    "الحسد يأكل الحسنات كما تأكل النار الحطب.",
    "من لا يشكر الناس لا يشكر الله.",
    "الصبر ضياء.",
    "بني الإسلام على خمس.",
    "الحياء لا يأتي إلا بخير.",
    "سلوا الله كل شيء حتى شسع النعل.",
    "من غشنا فليس منا.",
    "إن الله لا ينظر إلى أجسامكم ولكن إلى قلوبكم.",
    "إن الله مع الصابرين.",
    "الصدق يهدي إلى البر.",
    "لا تنزع الرحمة إلا من شقي."
};

// === Italien ===
static const char* hadiths_it[] = {
    "Le azioni sono giudicate dalle intenzioni.",
    "Il migliore tra voi è il più utile agli altri.",
    "Il Profeta era più generoso del vento.",
    "Un sorriso è una carità.",
    "Allah ama chi si pente.",
    "Il Paradiso è sotto i piedi delle madri.",
    "Chi crede in Allah dica il bene o taccia.",
    "La fede ha 70 rami.",
    "Ama per tuo fratello ciò che ami per te.",
    "L’invidia consuma le opere buone come il fuoco consuma il legno.",
    "Chi non ringrazia la gente non ringrazia Allah.",
    "La pazienza è luce.",
    "L’Islam è basato su cinque pilastri.",
    "La modestia porta all’amore di Allah.",
    "Chiedi ad Allah anche per il laccio della scarpa.",
    "Chi ci inganna non è dei nostri.",
    "Allah guarda i cuori, non i corpi.",
    "Allah è con i pazienti.",
    "La sincerità porta al Paradiso.",
    "La misericordia è tolta solo dai cuori duri."
};

const char* hadiths_get_today() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    if (!t) return "No date info";

    int index = t->tm_yday % 20;

    switch (config_get().language) {
        case LANG_FR: return hadiths_fr[index];
        case LANG_EN: return hadiths_en[index];
        case LANG_AR: return hadiths_ar[index];
        case LANG_IT: return hadiths_it[index];
        default: return hadiths_en[index];
    }
}

#ifdef __cplusplus
}
#endif
