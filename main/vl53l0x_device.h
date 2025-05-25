#ifndef _VL53L0X_DEVICE_H_
#define _VL53L0X_DEVICE_H_

#include <stdint.h>
#include "vl53l0x_def.h"             // ✅ Contient la définition complète de VL53L0X_DevData_t
#include "vl53l0x_device_struct.h"   // ✅ Contient les macros PALDevDataGet etc.

// Structure principale représentant le périphérique
typedef struct {
    uint8_t I2cDevAddr;
    uint8_t comms_type;
    uint16_t comms_speed_khz;

    VL53L0X_DevData_t Data;

    VL53L0X_DeviceSpecificParameters_t DeviceSpecificParameters;
} VL53L0X_Dev_t;

// Type opaque
typedef void* VL53L0X_DEV;

#endif // _VL53L0X_DEVICE_H_
