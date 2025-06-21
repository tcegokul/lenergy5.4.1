#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "TCA9554PWR.h"
#include "PCF85063.h"
#include "QMI8658.h"
#include "ST7701S.h"
#include "CST820.h"
#include "LVGL_Driver.h"
#include "LVGL_Example.h"
#include "Wireless.h"
#include "i2c_bus.h"
#include "tca9535.h"
#include "spi_bus.h"


#include "vl53l0x_sensor.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_api_ranging.h"



TimerHandle_t my_timer;
bool accel_Data_rdy = false;
uint8_t buf[6];
extern IMUdata Accel;
VL53L0X_RangingMeasurementData_t measurement1;

extern VL53L0X_Dev_t sensor ;
extern lv_obj_t *led;

void my_timer_callback(TimerHandle_t xTimer)
{
   // QMI8658_Loop();
    accel_Data_rdy = true;
}

uint16_t xshut_state = 0;

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


void app_main(void)
{ 
    //I2C_Init();

   // i2c_master_init();

    esp_tca9535_config_t pca_cfg = {
        .i2c_scl = GPIO_NUM_48,
        .i2c_sda = GPIO_NUM_47,
        .interrupt_output = -1,
    };
    tca9535_init(&pca_cfg);
   // QMI8658_Init();
    // spi_bus_init();

    tca9535_set_output_state(TCA9535_GPIO_NUM_11, TCA9535_IO_HIGH);

    vTaskDelay(pdMS_TO_TICKS(100));  // Wait at least 10ms

    xshut_state = tca9535_get_output_state(TCA9535_GPIO_NUM_11);

    ESP_LOGI("tca9535", "State: %u", xshut_state);

    i2c_scan();
    
    vl53l0x_init();

    LCD_Init();
    LVGL_Init();
/********************* Demo *********************/
    Lvgl_Example1();

      my_timer = xTimerCreate("MyTimer", pdMS_TO_TICKS(350), pdTRUE, NULL, my_timer_callback);

    if (my_timer != NULL)
    {
        xTimerStart(my_timer, 0);
    } 

    //lv_demo_widgets();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));
        lv_timer_handler();

        if (accel_Data_rdy)
         {
            accel_Data_rdy = false;
           // ESP_LOGI("APP", "Accel Data -> X: %.2f, Y: %.2f, Z: %.2f", Accel.x, Accel.y, Accel.z);

            VL53L0X_PerformSingleRangingMeasurement(&sensor, &measurement1);
            if((measurement1.RangeMilliMeter >= 150) && (measurement1.RangeMilliMeter != 0))
            lv_led_off(led);
            
            else if ((measurement1.RangeMilliMeter != 0) && (measurement1.RangeMilliMeter < 150))
            lv_led_on(led);

           // ESP_LOGI("VL53l0x", "Range: %u, Status: %u", measurement1.RangeMilliMeter, measurement1.RangeStatus);

            

            

        } 
 }
}
