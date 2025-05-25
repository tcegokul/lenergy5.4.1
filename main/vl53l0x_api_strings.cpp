#ifdef __cplusplus
extern "C" {
#endif

#include "vl53l0x_def.h"
#include "vl53l0x_device.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_platform_log.h"


#include "vl53l0x_api.h"
#include "vl53l0x_strings.h"


#ifdef __cplusplus
}
#endif


#include "vl53l0x_api_strings.h"



#define LOG_FUNCTION_START(fmt, ...) _LOG_FUNCTION_START(TRACE_MODULE_API, fmt, ##__VA_ARGS__)
#define LOG_FUNCTION_END(status, ...) _LOG_FUNCTION_END(TRACE_MODULE_API, status, ##__VA_ARGS__)

VL53L0X_Error VL53L0X_get_pal_error_string(VL53L0X_Error PalErrorCode, char *pPalErrorString)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    LOG_FUNCTION_START("");

    switch (PalErrorCode) {
    case VL53L0X_ERROR_NONE:
        VL53L0X_COPYSTRING(pPalErrorString, VL53L0X_STRING_ERROR_NONE);
        break;
    case VL53L0X_ERROR_CALIBRATION_WARNING:
        VL53L0X_COPYSTRING(pPalErrorString, VL53L0X_STRING_ERROR_CALIBRATION_WARNING);
        break;
    default:
        VL53L0X_COPYSTRING(pPalErrorString, VL53L0X_STRING_UNKNOW_ERROR_CODE);
        break;
    }

    LOG_FUNCTION_END(Status);
    return Status;
}

VL53L0X_Error VL53L0X_get_pal_state_string(VL53L0X_State PalStateCode, char *pPalStateString)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    LOG_FUNCTION_START("");

    switch (PalStateCode) {
    case VL53L0X_STATE_POWERDOWN:
        VL53L0X_COPYSTRING(pPalStateString, VL53L0X_STRING_STATE_POWERDOWN);
        break;
    case VL53L0X_STATE_WAIT_STATICINIT:
        VL53L0X_COPYSTRING(pPalStateString, VL53L0X_STRING_STATE_WAIT_STATICINIT);
        break;
    default:
        VL53L0X_COPYSTRING(pPalStateString, VL53L0X_STRING_UNKNOWN_STATE_CODE);
        break;
    }

    LOG_FUNCTION_END(Status);
    return Status;
}

VL53L0X_Error VL53L0X_get_sequence_steps_info(VL53L0X_SequenceStepId SequenceStepId, char *pSequenceStepsString)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    LOG_FUNCTION_START("");

    switch (SequenceStepId) {
    case VL53L0X_SEQUENCESTEP_TCC:
        VL53L0X_COPYSTRING(pSequenceStepsString, VL53L0X_STRING_SEQUENCESTEP_TCC);
        break;
    case VL53L0X_SEQUENCESTEP_DSS:
        VL53L0X_COPYSTRING(pSequenceStepsString, VL53L0X_STRING_SEQUENCESTEP_DSS);
        break;
    default:
        VL53L0X_COPYSTRING(pSequenceStepsString, VL53L0X_STRING_UNKNOWN_SEQUENCE_STEP_ID);
        break;
    }

    LOG_FUNCTION_END(Status);
    return Status;
}

VL53L0X_Error VL53L0X_get_limit_check_info(VL53L0X_DEV Dev, uint16_t LimitCheckId, char *pLimitCheckString)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    LOG_FUNCTION_START("");

    switch (LimitCheckId) {
    case VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE:
        VL53L0X_COPYSTRING(pLimitCheckString, VL53L0X_STRING_CHECKENABLE_SIGMA_FINAL_RANGE);
        break;
    case VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE:
        VL53L0X_COPYSTRING(pLimitCheckString, VL53L0X_STRING_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE);
        break;
    default:
        VL53L0X_COPYSTRING(pLimitCheckString, VL53L0X_STRING_UNKNOWN_LIMIT_CHECK_ID);
        break;
    }

    LOG_FUNCTION_END(Status);
    return Status;
}

