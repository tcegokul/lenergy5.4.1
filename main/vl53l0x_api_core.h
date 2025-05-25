#ifndef _VL53L0X_API_CORE_H_
#define _VL53L0X_API_CORE_H_

#include "vl53l0x_def.h"
#include "vl53l0x_device.h"  // Utilisation de la structure SpadData
#include "vl53l0x_types.h"


#include "vl53l0x_error_codes.h"


#ifdef __cplusplus
extern "C" {
#endif

VL53L0X_Error VL53L0X_perform_single_ref_calibration(VL53L0X_DEV Dev,
                                                     uint8_t vhv_init_byte,
                                                     uint8_t phase_cal);

VL53L0X_Error VL53L0X_measurement_poll_for_completion(VL53L0X_DEV Dev);

VL53L0X_Error VL53L0X_get_info_from_device(VL53L0X_DEV Dev, uint8_t Option);


VL53L0X_Error VL53L0X_get_pal_range_status(VL53L0X_DEV Dev,
                                           VL53L0X_RangingMeasurementData_t *pRangingMeasurementData,
                                           uint8_t *pPalRangeStatus);


// ✅ À ajouter pour résoudre les appels depuis vl53l0x_api.cpp

VL53L0X_Error VL53L0X_set_offset_calibration_data_micro_meter(VL53L0X_DEV Dev, int32_t Offset);
VL53L0X_Error VL53L0X_get_total_signal_rate(VL53L0X_DEV Dev, VL53L0X_RangingMeasurementData_t *pData, FixPoint1616_t *pRate);
VL53L0X_Error VL53L0X_set_measurement_timing_budget_micro_seconds(VL53L0X_DEV Dev, uint32_t Budget);
VL53L0X_Error VL53L0X_set_vcsel_pulse_period(VL53L0X_DEV Dev, VL53L0X_VcselPeriod type, uint8_t period);
VL53L0X_Error VL53L0X_get_sequence_step_timeout(VL53L0X_DEV Dev, VL53L0X_SequenceStepId id, FixPoint1616_t* timeout);

// ✅ Fonctions à implémenter également dans vl53l0x_api_core.cpp
VL53L0X_Error VL53L0X_set_ref_calibration(VL53L0X_DEV Dev, uint8_t vhv, uint8_t phase);
VL53L0X_Error VL53L0X_perform_ref_calibration(VL53L0X_DEV Dev, uint8_t* vhv, uint8_t* phase);
VL53L0X_Error VL53L0X_perform_xtalk_calibration(VL53L0X_DEV Dev, FixPoint1616_t distance, FixPoint1616_t* xtalk);
VL53L0X_Error VL53L0X_perform_offset_calibration(VL53L0X_DEV Dev, FixPoint1616_t distance, int32_t* offset);
VL53L0X_Error VL53L0X_perform_ref_spad_management(VL53L0X_DEV Dev, uint32_t* count, uint8_t* is_aperture);

VL53L0X_Error VL53L0X_get_measurement_timing_budget_micro_seconds(VL53L0X_DEV Dev, uint32_t* pBudget);
VL53L0X_Error VL53L0X_get_sequence_step_timeout(VL53L0X_DEV Dev, VL53L0X_SequenceStepId SequenceStepId, FixPoint1616_t* pTimeoutMicroSeconds);


#ifdef __cplusplus
}
#endif

#endif /* _VL53L0X_API_CORE_H_ */




