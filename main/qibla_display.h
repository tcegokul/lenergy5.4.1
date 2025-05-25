
// qibla_display.h
#ifndef QIBLA_DISPLAY_H
#define QIBLA_DISPLAY_H

#include <stdbool.h>
#include "image_manager.h"

// Affichage principal (celle utilisée dans main.cpp)
void qibla_display(void);

// Variante avec données GPS directement fournies
void qibla_display_with_data(float bearing, float distance);

// Initialisation
void qibla_display_init(void);

// Mise à jour en fonction de la boussole uniquement
void qibla_display_update(float heading_deg);

// Calcul de l’angle vers la Qibla
float qibla_display_get_qibla_angle(void);

// Vérifie si les données Qibla sont fiables (fix GPS et boussole)
bool qibla_display_has_fix(void);

#endif // QIBLA_DISPLAY_H

