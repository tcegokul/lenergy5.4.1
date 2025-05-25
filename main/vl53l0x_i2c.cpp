#ifdef __cplusplus
extern "C" {
#endif

#include "vl53l0x_def.h"
#include "vl53l0x_device.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_platform_log.h"
#include "vl53l0x_api.h"


#ifdef __cplusplus
}
#endif

#include <driver/i2c.h>
#include "vl53l0x_i2c.h"

// Adresse I2C du capteur VL53L0X
#define VL53L0X_I2C_ADDRESS 0x29
#define I2C_PORT I2C_NUM_0

int vl53l0x_espidf_write_multi(uint8_t deviceAddr, uint8_t reg, uint8_t *pdata, uint32_t count) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write(cmd, pdata, count, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return (ret == ESP_OK) ? 0 : -1;
}

int vl53l0x_espidf_read_multi(uint8_t deviceAddr, uint8_t reg, uint8_t *pdata, uint32_t count) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddr << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, pdata, count, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return (ret == ESP_OK) ? 0 : -1;
}

int vl53l0x_espidf_write_byte(uint8_t address, uint8_t reg, uint8_t data) {
    return vl53l0x_espidf_write_multi(address, reg, &data, 1);
}

int vl53l0x_espidf_write_word(uint8_t address, uint8_t reg, uint16_t data) {
    uint8_t buffer[2] = { (uint8_t)(data >> 8), (uint8_t)(data & 0xFF) };
    return vl53l0x_espidf_write_multi(address, reg, buffer, 2);
}

int vl53l0x_espidf_read_byte(uint8_t address, uint8_t reg, uint8_t *data) {
    return vl53l0x_espidf_read_multi(address, reg, data, 1);
}

int vl53l0x_espidf_read_word(uint8_t address, uint8_t reg, uint16_t *data) {
    uint8_t buffer[2];
    int status = vl53l0x_espidf_read_multi(address, reg, buffer, 2);
    if (status == 0)
        *data = ((uint16_t)buffer[0] << 8) | buffer[1];
    return status;
}

int vl53l0x_espidf_read_dword(uint8_t address, uint8_t reg, uint32_t *data) {
    uint8_t buffer[4];
    int status = vl53l0x_espidf_read_multi(address, reg, buffer, 4);
    if (status == 0)
        *data = ((uint32_t)buffer[0] << 24) | ((uint32_t)buffer[1] << 16) |
                ((uint32_t)buffer[2] << 8) | buffer[3];
    return status;
}
