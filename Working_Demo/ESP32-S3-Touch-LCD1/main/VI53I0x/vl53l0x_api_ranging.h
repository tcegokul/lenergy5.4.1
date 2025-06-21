
/*******************************************************************************
 * VL53L0X_api_ranging.h
 * Header file for ranging API - STMicroelectronics VL53L0X
 *******************************************************************************/

#ifndef _VL53L0X_API_RANGING_H_
#define _VL53L0X_API_RANGING_H_

#include "vl53l0x_def.h"
#include "vl53l0x_device.h"  // Utilisation de la structure SpadData


#ifdef __cplusplus
extern "C" {
#endif

VL53L0X_Error VL53L0X_PerformSingleRangingMeasurement(
    VL53L0X_DEV Dev,
    VL53L0X_RangingMeasurementData_t *pRangingMeasurementData);

VL53L0X_Error VL53L0X_StartMeasurement(VL53L0X_DEV Dev);
VL53L0X_Error VL53L0X_StopMeasurement(VL53L0X_DEV Dev);
VL53L0X_Error VL53L0X_GetMeasurementDataReady(VL53L0X_DEV Dev, uint8_t *pMeasurementDataReady);
VL53L0X_Error VL53L0X_GetRangingMeasurementData(VL53L0X_DEV Dev, VL53L0X_RangingMeasurementData_t *pRangingMeasurementData);
VL53L0X_Error VL53L0X_ClearInterruptMask(VL53L0X_DEV Dev, uint32_t InterruptMask);
VL53L0X_Error VL53L0X_GetInterruptMaskStatus(VL53L0X_DEV Dev, uint32_t *pInterruptMaskStatus);

#ifdef __cplusplus
}
#endif

#endif // _VL53L0X_API_RANGING_H_
