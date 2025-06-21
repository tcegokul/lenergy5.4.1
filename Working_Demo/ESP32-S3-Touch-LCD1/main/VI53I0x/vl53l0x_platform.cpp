#include "vl53l0x_def.h"
#include "vl53l0x_device.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_platform_log.h"
#include "vl53l0x_api.h"
#include <driver/i2c.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include <string.h>

#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_TIMEOUT_MS 1000
uint32_t _trace_level = TRACE_LEVEL_NONE;
extern "C" {

typedef struct {
    uint8_t I2cDevAddr;
} VL53L0X_Device;

esp_err_t i2c_master_init() {
    
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_8,
        .scl_io_num = GPIO_NUM_9,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
        .clk_speed = 400000,
        },
        .clk_flags = 0
    };
    esp_err_t ret = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (ret != ESP_OK) return ret;

    return i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0);
}

VL53L0X_Error VL53L0X_WriteMulti(VL53L0X_DEV dev, uint8_t index, uint8_t *pdata, uint32_t count) {
    uint8_t deviceAddress = ((VL53L0X_Device*)dev)->I2cDevAddr;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddress << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, index, true);
    i2c_master_write(cmd, pdata, count, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return (ret == ESP_OK) ? VL53L0X_ERROR_NONE : VL53L0X_ERROR_CONTROL_INTERFACE;
}

VL53L0X_Error VL53L0X_ReadMulti(VL53L0X_DEV dev, uint8_t index, uint8_t *pdata, uint32_t count) {
    uint8_t deviceAddress = ((VL53L0X_Device*)dev)->I2cDevAddr;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddress << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, index, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddress << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, pdata, count, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return (ret == ESP_OK) ? VL53L0X_ERROR_NONE : VL53L0X_ERROR_CONTROL_INTERFACE;
}

VL53L0X_Error VL53L0X_WrByte(VL53L0X_DEV dev, uint8_t index, uint8_t data) {
    return VL53L0X_WriteMulti(dev, index, &data, 1);
}

VL53L0X_Error VL53L0X_WrWord(VL53L0X_DEV dev, uint8_t index, uint16_t data) {
    uint8_t buffer[2] = { (uint8_t)(data >> 8), (uint8_t)(data & 0xFF) };
    return VL53L0X_WriteMulti(dev, index, buffer, 2);
}

VL53L0X_Error VL53L0X_WrDWord(VL53L0X_DEV dev, uint8_t index, uint32_t data) {
    uint8_t buffer[4] = {
        (uint8_t)(data >> 24),
        (uint8_t)(data >> 16),
        (uint8_t)(data >> 8),
        (uint8_t)(data & 0xFF)
    };
    return VL53L0X_WriteMulti(dev, index, buffer, 4);
}

VL53L0X_Error VL53L0X_UpdateByte(VL53L0X_DEV dev, uint8_t index, uint8_t and_data, uint8_t or_data) {
    uint8_t data;
    VL53L0X_Error status = VL53L0X_ReadMulti(dev, index, &data, 1);
    if (status != VL53L0X_ERROR_NONE) return status;

    data = (data & and_data) | or_data;
    return VL53L0X_WrByte(dev, index, data);
}

VL53L0X_Error VL53L0X_RdByte(VL53L0X_DEV dev, uint8_t index, uint8_t *data) {
    return VL53L0X_ReadMulti(dev, index, data, 1);
}

VL53L0X_Error VL53L0X_RdWord(VL53L0X_DEV dev, uint8_t index, uint16_t *data) {
    uint8_t buffer[2];
    VL53L0X_Error status = VL53L0X_ReadMulti(dev, index, buffer, 2);
    if (status == VL53L0X_ERROR_NONE) {
        *data = ((uint16_t)buffer[0] << 8) | buffer[1];
    }
    return status;
}

VL53L0X_Error VL53L0X_RdDWord(VL53L0X_DEV dev, uint8_t index, uint32_t *data) {
    uint8_t buffer[4];
    VL53L0X_Error status = VL53L0X_ReadMulti(dev, index, buffer, 4);
    if (status == VL53L0X_ERROR_NONE) {
        *data = ((uint32_t)buffer[0] << 24) |
                ((uint32_t)buffer[1] << 16) |
                ((uint32_t)buffer[2] << 8) |
                buffer[3];
    }
    return status;
}

VL53L0X_Error VL53L0X_PollingDelay(VL53L0X_DEV dev) {
    vTaskDelay(1 / portTICK_PERIOD_MS);  // Petit délai (1 tick)
    return VL53L0X_ERROR_NONE;
}

}