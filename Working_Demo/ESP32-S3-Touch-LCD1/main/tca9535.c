/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2021 <ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <string.h>
#include "driver/gpio.h"
#include "i2c_bus.h"
#include "esp_log.h"
#include "tca9535.h"

static char *TAG = "TCA9535";
static i2c_bus_handle_t i2c_handle;

#define SET_BITS(_m, _s, _v)  ((_v) ? (_m)|((_s)) : (_m)&~((_s)))
#define GET_BITS(_m, _s)      (((_m) & (_s)) ? true : false)

#define TCA9535_INPUT_PORT0                 0x00
#define TCA9535_INPUT_PORT1                 0x01
#define TCA9535_OUTPUT_PORT0                0x02
#define TCA9535_OUTPUT_PORT1                0x03
#define TCA9535_POLARITY_INVERSION_PORT0    0x04
#define TCA9535_POLARITY_INVERSION_PORT1    0x05
#define TCA9535_CONFIGURATION_PORT0         0x06
#define TCA9535_CONFIGURATION_PORT1         0x07

typedef struct {
    uint8_t addr;
    char *name;
} tca9535_dev_t;

static tca9535_dev_t dev_list[] = {
    { 0x40, "TCA9535"},
};

static uint8_t tca9535_addr;

static esp_err_t expander_dev_prob()
{
    for (size_t i = 0; i < sizeof(dev_list) / sizeof(dev_list[0]); i++) {

        if (ESP_OK == i2c_bus_probe_addr(i2c_handle, dev_list[i].addr)) {
            ESP_LOGI(TAG, "Detected IO expander device at 0x%02X, name is: %s", dev_list[i].addr, dev_list[i].name);
            tca9535_addr = dev_list[i].addr;
            return ESP_OK;
        }
    }
    tca9535_addr = 0;
    ESP_LOGE(TAG, "IO expander device has not detected");
    return ESP_ERR_NOT_FOUND;
}


static esp_err_t tca9535_write_reg(uint8_t reg_addr, uint8_t data)
{
    return i2c_bus_write_bytes(i2c_handle, tca9535_addr, &reg_addr, sizeof(reg_addr), &data, sizeof(data));
}

static char tca9535_read_reg(uint8_t reg_addr)
{
    uint8_t data;
    i2c_bus_read_bytes(i2c_handle, tca9535_addr, &reg_addr, sizeof(reg_addr), &data, sizeof(data));
    return data;
}

static int i2c_init(gpio_num_t clk, gpio_num_t sda)
{
    int res = 0;
    i2c_config_t es_i2c_cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 800000,
        .scl_io_num = clk,
        .sda_io_num = sda,
    };
    i2c_handle = i2c_bus_create(I2C_NUM_0, &es_i2c_cfg);
    return res;
}

esp_tca9535_io_level_t tca9535_get_input_state(esp_tca9535_gpio_num_t gpio_num)
{
    uint16_t data = 0x00;
    if (gpio_num < TCA9535_GPIO_NUM_MAX) {
        data |= tca9535_read_reg(TCA9535_INPUT_PORT0) << 0x00;
        data |= tca9535_read_reg(TCA9535_INPUT_PORT1) << 0x08;
    } else {
        ESP_LOGE(TAG, "gpio num is error, current gpio: %d", gpio_num);
        return TCA9535_LEVEL_ERROR;
    }

    return GET_BITS(data, gpio_num);
}

esp_tca9535_io_level_t tca9535_get_output_state(esp_tca9535_gpio_num_t gpio_num)
{
    uint16_t data = 0x00;
    if (gpio_num < TCA9535_GPIO_NUM_MAX) {
        data |= tca9535_read_reg(TCA9535_OUTPUT_PORT0) << 0x00;
        data |= tca9535_read_reg(TCA9535_OUTPUT_PORT1) << 0x08;
    } else {
        ESP_LOGE(TAG, "gpio num is error, current gpio: %d", gpio_num);
        return TCA9535_LEVEL_ERROR;
    }

    return GET_BITS(data, gpio_num);
}

esp_err_t tca9535_set_output_state(esp_tca9535_gpio_num_t gpio_num, esp_tca9535_io_level_t level)
{
    uint16_t data = 0x00;
    esp_err_t res = ESP_FAIL;
    if (gpio_num < TCA9535_GPIO_NUM_MAX) {
        data |= tca9535_read_reg(TCA9535_OUTPUT_PORT0) << 0x00;
        data |= tca9535_read_reg(TCA9535_OUTPUT_PORT1) << 0x08;
        res = tca9535_write_reg(TCA9535_OUTPUT_PORT0, (SET_BITS(data, gpio_num, level) >> 0x00) & 0xFF);
        res = tca9535_write_reg(TCA9535_OUTPUT_PORT1, (SET_BITS(data, gpio_num, level) >> 0x08) & 0xFF);
    } else {
        ESP_LOGE(TAG, "gpio num is error, current gpio: %d", gpio_num);
    }
    return res;
}

esp_err_t tca9535_set_polarity_inversion(esp_tca9535_gpio_num_t gpio_num, esp_tca9535_io_polarity_t polarity)
{
    uint16_t data = 0x00;
    esp_err_t res = ESP_FAIL;
    if (gpio_num < TCA9535_GPIO_NUM_MAX) {
        data |= tca9535_read_reg(TCA9535_POLARITY_INVERSION_PORT0) << 0x00;
        data |= tca9535_read_reg(TCA9535_POLARITY_INVERSION_PORT1) << 0x08;
        res = tca9535_write_reg(TCA9535_POLARITY_INVERSION_PORT0, (SET_BITS(data, gpio_num, polarity) >> 0x00) & 0xFF);
        res = tca9535_write_reg(TCA9535_POLARITY_INVERSION_PORT1, (SET_BITS(data, gpio_num, polarity) >> 0x08) & 0xFF);
    } else {
        ESP_LOGE(TAG, "gpio num is error, current gpio: %d", gpio_num);
    }
    return res;
}

esp_tca9535_io_config_t tca9535_get_io_config(esp_tca9535_gpio_num_t gpio_num)
{
    uint16_t data = 0x00;
    if (gpio_num < TCA9535_GPIO_NUM_MAX) {
        data |= tca9535_read_reg(TCA9535_CONFIGURATION_PORT0) << 0x00;
        data |= tca9535_read_reg(TCA9535_CONFIGURATION_PORT1) << 0x08;
    } else {
        ESP_LOGE(TAG, "gpio num is error, current gpio: %d", gpio_num);
        return TCA9535_LEVEL_ERROR;
    }

    return GET_BITS(data, gpio_num);
}

esp_err_t tca9535_set_io_config(esp_tca9535_gpio_num_t gpio_num, esp_tca9535_io_config_t io_config)
{
    uint16_t data = 0x00;
    esp_err_t res = ESP_FAIL;
    if (gpio_num < TCA9535_GPIO_NUM_MAX) {
        data |= tca9535_read_reg(TCA9535_CONFIGURATION_PORT0) << 0x00;
        data |= tca9535_read_reg(TCA9535_CONFIGURATION_PORT1) << 0x08;
        res = tca9535_write_reg(TCA9535_CONFIGURATION_PORT0, (SET_BITS(data, gpio_num, io_config) >> 0x00) & 0xFF);
        res = tca9535_write_reg(TCA9535_CONFIGURATION_PORT1, (SET_BITS(data, gpio_num, io_config) >> 0x08) & 0xFF);
    } else {
        ESP_LOGE(TAG, "gpio num is error, current gpio: %d", gpio_num);
    }
    return res;
}

esp_err_t tca9535_init(esp_tca9535_config_t *cfg)
{
    esp_err_t ret = ESP_OK;
    i2c_init(cfg->i2c_scl, cfg->i2c_sda);

    if (ESP_OK == expander_dev_prob()) {
        tca9535_write_reg(0x06, 0x00);
        tca9535_write_reg(0x07, 0x00);
        tca9535_write_reg(0x02, 0xFF);
        tca9535_write_reg(0x03, 0xFF);
    }

    return ret;
}

esp_err_t tca9535_deinit()
{
    i2c_bus_delete(i2c_handle);
    return ESP_OK;
}
