#ifndef _VL53L0X_REGISTER_SETTINGS_H_
#define _VL53L0X_REGISTER_SETTINGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Register addresses */
#define VL53L0X_REG_SYSRANGE_START                                0x00
#define VL53L0X_REG_SYSTEM_THRESH_HIGH                            0x0E
#define VL53L0X_REG_SYSTEM_THRESH_LOW                             0x0C
#define VL53L0X_REG_SYSTEM_SEQUENCE_CONFIG                        0x01
#define VL53L0X_REG_SYSTEM_RANGE_CONFIG                           0x09
#define VL53L0X_REG_SYSTEM_INTERMEASUREMENT_PERIOD                0x04
#define VL53L0X_REG_SYSTEM_INTERRUPT_CONFIG_GPIO                  0x0A

#define VL53L0X_REG_GPIO_HV_MUX_ACTIVE_HIGH                       0x84
#define VL53L0X_REG_SYSTEM_INTERRUPT_CLEAR                        0x0B
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS                       0x13
#define VL53L0X_REG_RESULT_RANGE_STATUS                           0x14

#define VL53L0X_REG_CROSSTALK_COMPENSATION_PEAK_RATE_MCPS         0x20
#define VL53L0X_REG_PRE_RANGE_MIN_COUNT_RATE_RTN_LIMIT            0x64
#define VL53L0X_REG_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT   0x44
#define VL53L0X_REG_OSC_CALIBRATE_VAL                             0xF8

#define VL53L0X_REG_IDENTIFICATION_MODEL_ID                       0xC0
#define VL53L0X_REG_SOFT_RESET_GO2_SOFT_RESET_N                   0xBF
#define VL53L0X_REG_SYSTEM_RANGE_CONFIG                           0x09
#define VL53L0X_REG_SYSRANGE_MODE_START_STOP                      0x01
#define VL53L0X_REG_SYSRANGE_MODE_BACKTOBACK                      0x02
#define VL53L0X_REG_SYSRANGE_MODE_SINGLESHOT                      0x00
#define VL53L0X_REG_SYSRANGE_MODE_TIMED                           0x08


#define VL53L0X_REG_IDENTIFICATION_REVISION_ID 0x00 // vérifier la bonne valeur
#define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS   0x8A
#define VL53L0X_REG_MSRC_CONFIG_CONTROL        0x60



#ifdef __cplusplus
}
#endif

#endif /* _VL53L0X_REGISTER_SETTINGS_H_ */
