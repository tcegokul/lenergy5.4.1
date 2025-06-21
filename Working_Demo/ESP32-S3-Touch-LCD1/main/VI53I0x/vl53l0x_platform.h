// vl53l0x_platform.h
#ifndef VL53L0X_PLATFORM_H
#define VL53L0X_PLATFORM_H

#include "vl53l0x_def.h"
#include "vl53l0x_device.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif


#define VL53L0X_I2C_PORT I2C_NUM_1
#define VL53L0X_I2C_FREQ 400000

esp_err_t i2c_master_init(void);

VL53L0X_Error VL53L0X_WriteMulti(VL53L0X_DEV dev, uint8_t index, uint8_t *pdata, uint32_t count);
VL53L0X_Error VL53L0X_ReadMulti(VL53L0X_DEV dev, uint8_t index, uint8_t *pdata, uint32_t count);
VL53L0X_Error VL53L0X_WrByte(VL53L0X_DEV dev, uint8_t index, uint8_t data);
VL53L0X_Error VL53L0X_RdByte(VL53L0X_DEV dev, uint8_t index, uint8_t *data);
VL53L0X_Error VL53L0X_WrWord(VL53L0X_DEV dev, uint8_t index, uint16_t data);
VL53L0X_Error VL53L0X_RdWord(VL53L0X_DEV dev, uint8_t index, uint16_t *data);
VL53L0X_Error VL53L0X_WrDWord(VL53L0X_DEV dev, uint8_t index, uint32_t data);
VL53L0X_Error VL53L0X_RdDWord(VL53L0X_DEV dev, uint8_t index, uint32_t *data);
VL53L0X_Error VL53L0X_WaitMs(VL53L0X_DEV dev, int32_t wait_ms);
VL53L0X_Error VL53L0X_UpdateByte(VL53L0X_DEV dev, uint8_t index, uint8_t AndData, uint8_t OrData);
VL53L0X_Error VL53L0X_PollingDelay(VL53L0X_DEV dev);

#ifdef __cplusplus
}
#endif

#endif // VL53L0X_PLATFORM_H

