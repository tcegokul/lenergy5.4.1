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
#include "gps_parser.h"

#include "vl53l0x_sensor.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_api_ranging.h"

#include "counter.h"

#include "LVGL_UI/LVGL_CounterPage.h"


TimerHandle_t my_timer;
bool accel_Data_rdy = false;
uint8_t buf[6];
extern IMUdata Accel;
VL53L0X_RangingMeasurementData_t measurement1;

extern VL53L0X_Dev_t sensor ;
extern lv_obj_t *led;



prayer_state_t current_state = STATE_QIYAM;
prayer_state_t previous_state = STATE_QIYAM;

uint16_t previous_distance = 2000;
uint8_t rakah_counter = 0;


 void process_prayer_state(uint16_t distance_mm, uint8_t status)
{
    ESP_LOGI("DEBUG", "Distance: %u mm, Status: %u", distance_mm, status);

    if (status != 0) {
        ESP_LOGW("SKIP", "Invalid measurement, skipping...");
        return;
    }

    prayer_state_t detected_posture = current_state; // Default to current unless valid change

    // Context-aware posture detection
    switch (current_state) {
        case STATE_QIYAM:
            if (distance_mm >= 500 && distance_mm < 1000)
                detected_posture = STATE_RUKU;
            break;

        case STATE_RUKU:
            if (distance_mm >= 50 && distance_mm < 150)
                detected_posture = STATE_SUJOOD_1;
            break;

        case STATE_SUJOOD_1:
            if (distance_mm == 0 || distance_mm >= 1000)
                detected_posture = STATE_JULLOS;
            break;

        case STATE_JULLOS:
            if (distance_mm >= 50 && distance_mm < 150)
                detected_posture = STATE_SUJOOD_2;
            break;

        case STATE_SUJOOD_2:
            if (distance_mm == 0 || distance_mm >= 1000)
                detected_posture = STATE_QIYAM;
            break;

        default:
            break;
    }

    // Always log what posture sensor sees
    const char* posture_names[] = {
        "Qiyam", "Ruku", "Sujood 1", "Jullos", "Sujood 2"
    };
    ESP_LOGI("POSTURE", "Sensor infers posture: %s", posture_names[detected_posture]);

    // Check transition
    if (detected_posture != current_state) {
        previous_state = current_state;
        current_state = detected_posture;

        ESP_LOGI("PRAYER", "Transitioned from %s to %s",
                 posture_names[previous_state],
                 posture_names[current_state]);

        // Increment rak‘ah after completing cycle
        if (current_state == STATE_SUJOOD_2 && previous_state == STATE_JULLOS) {
            rakah_counter++;
            ESP_LOGI("RAKAH", "Rak‘ah completed: %d", rakah_counter);
        }
    }
}
 

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

void gps_update()
{
    if(gps_has_fix()){
        gps_get_local_time();
        ESP_LOGI("GPS", "Local Time: %s", gps_get_local_time());
    }
    else {
        ESP_LOGI("GPS", "No GPS Fix");
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

   // gps_parser_init(0);
   
    //gps_update();

    LCD_Init();
    LVGL_Init();
/********************* Demo *********************/
    Lvgl_Example1();

    //Lvgl_Example1_close();
    //show_start_screen();

    lv_obj_t *counter_screen = lv_obj_create(NULL);
    counter_page_create(counter_screen);
    lv_scr_load(counter_screen);


      my_timer = xTimerCreate("MyTimer", pdMS_TO_TICKS(1000), pdTRUE, NULL, my_timer_callback);

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
            
           VL53L0X_PerformSingleRangingMeasurement(&sensor, &measurement1);
             uint16_t distance = measurement1.RangeMilliMeter;
             uint8_t status = measurement1.RangeStatus;


            
             process_prayer_state(distance, status);
             update_prayer_ui(rakah_counter, current_state);

            

         } 

       
}
}
