#ifndef VL53L0X_INTERRUPT_THRESHOLD_SETTINGS_H
#define VL53L0X_INTERRUPT_THRESHOLD_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Données de seuils d'interruption par défaut pour le VL53L0X.
// Ces valeurs sont typiquement utilisées pour des démos ou comme valeurs par défaut dans le tuning.
static const uint8_t InterruptThresholdSettings[] = {
    // Exemple de données (remplacer si vous avez des valeurs spécifiques)
    0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C,
    0x0D, 0x0E, 0x0F, 0x10
};

#ifdef __cplusplus
}
#endif

#endif // VL53L0X_INTERRUPT_THRESHOLD_SETTINGS_H
