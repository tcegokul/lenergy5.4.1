#include "defintions.h"


 void i2c_scan()
{
    for (uint8_t addr = 1; addr < 127; addr++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);
        esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);
        if (ret == ESP_OK) {
            ESP_LOGI("i2c_scan", "Found device at 0x%02X", addr);
        }
    }
}

 void system_initalise()
 {
     tca9535_pinconfig_();
     tca9535_set_output_state(TCA9535_GPIO_NUM_11, TCA9535_IO_HIGH);

    i2c_scan();
    
    vl53l0x_init();

   
    LCD_Init();
    LVGL_Init();

 }
 
