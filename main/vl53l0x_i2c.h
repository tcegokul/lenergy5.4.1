#ifndef VL53L0X_I2C_ESP_IDF_H
#define VL53L0X_I2C_ESP_IDF_H

#ifdef __cplusplus
extern "C" {
#endif

int vl53l0x_espidf_write_multi(uint8_t deviceAddr, uint8_t reg, uint8_t *pdata, uint32_t count);
int vl53l0x_espidf_read_multi(uint8_t deviceAddr, uint8_t reg, uint8_t *pdata, uint32_t count);
int vl53l0x_espidf_write_byte(uint8_t address, uint8_t reg, uint8_t data);
int vl53l0x_espidf_write_word(uint8_t address, uint8_t reg, uint16_t data);
int vl53l0x_espidf_read_byte(uint8_t address, uint8_t reg, uint8_t *data);
int vl53l0x_espidf_read_word(uint8_t address, uint8_t reg, uint16_t *data);
int vl53l0x_espidf_read_dword(uint8_t address, uint8_t reg, uint32_t *data);

#ifdef __cplusplus
}
#endif

#endif
