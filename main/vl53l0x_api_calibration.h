#ifndef _VL53L0X_API_CALIBRATION_H_
#define _VL53L0X_API_CALIBRATION_H_

#include "vl53l0x_def.h"
#include "vl53l0x_device.h"  // Utilisation de la structure SpadData


#ifdef __cplusplus
extern "C" {
#endif

// ✅ Ne garder que celles non présentes dans vl53l0x_api.h :
VL53L0X_Error VL53L0X_set_reference_spads(VL53L0X_DEV Dev, uint32_t count, uint8_t is_aperture);
VL53L0X_Error VL53L0X_get_reference_spads(VL53L0X_DEV Dev, uint32_t* pSpadCount, uint8_t* pIsApertureSpads);

#ifdef __cplusplus
}
#endif

#endif // _VL53L0X_API_CALIBRATION_H_
