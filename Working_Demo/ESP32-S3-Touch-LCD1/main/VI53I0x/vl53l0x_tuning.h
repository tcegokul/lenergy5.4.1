#ifndef _VL53L0X_TUNING_H_
#define _VL53L0X_TUNING_H_

#include "vl53l0x_def.h"  // Pour le type VL53L0X_DEV

#define VL53L0X_TUNINGPARM_VERSION 0
#define VL53L0X_TUNINGPARM_GENERAL_CONFIG 1
#define VL53L0X_TUNINGPARM_MM_CONFIG_TIMEOUT_US 2
#define VL53L0X_TUNINGPARM_RANGE_CONFIG_TIMEOUT_US 3
#define VL53L0X_TUNINGPARM_EXPECTED_TARGET_COUNT_RATE_MCPS 4
#define VL53L0X_TUNINGPARM_SIGMA_ESTIMATE_MCPS 5
#define VL53L0X_TUNINGPARM_SIGNAL_ESTIMATE_MCPS 6
#define VL53L0X_TUNINGPARM_REF_SIGNAL_MCPS 7
#define VL53L0X_TUNINGPARM_REF_DISTANCE_MM 8
#define VL53L0X_TUNINGPARM_MM_TIMEOUT_CALC 9
#define VL53L0X_TUNINGPARM_RANGE_TIMEOUT_CALC 10
#define VL53L0X_TUNINGPARM_MAX 11

#ifdef __cplusplus
extern "C" {
#endif

// ✅ Déclaration manquante à ajouter ici :
VL53L0X_Error VL53L0X_LoadTuningSettings(VL53L0X_DEV Dev, const uint8_t *pTuningSettingBuffer);


#ifdef __cplusplus
}
#endif

#endif // _VL53L0X_TUNING_H_
