
#include "vl53l0x_api.h"
#include "vl53l0x_def.h"
#include "vl53l0x_platform.h"

// Tableau de tuning par défaut minimal
const uint8_t VL53L0X_DefaultTuningSettings[] = { 0x00 };


// Stub pour l’état de la mesure, utilisé par VL53L0X_API
VL53L0X_Error VL53L0X_get_pal_range_status(VL53L0X_DEV Dev, uint8_t *pRangeStatus) {
    if (pRangeStatus) {
        *pRangeStatus = 0; // 0 = Valid Range
    }
    return VL53L0X_ERROR_NONE;
}
