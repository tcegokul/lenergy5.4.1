#pragma once
/* Central “umbrella” header for app sources.
 * Use in .c files; avoid pulling this into public library headers.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* --- C library --- */
#include <stdio.h>
#include <string.h>

/* --- Project config (must precede most ESP-IDF headers) --- */
#include "sdkconfig.h"

/* --- RTOS --- */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* --- ESP-IDF core --- */
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"

/* --- NVS / storage --- */
#include "nvs_flash.h"

/* --- Device drivers / BSP --- */
#include "i2c_bus.h"
#include "tca9535.h"
#include "TCA9554PWR.h"
#include "spi_bus.h"
#include "ST7701S.h"
#include "CST820.h"

/* --- Sensors (VL53L0X) --- */
#include "vl53l0x_sensor.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_api_ranging.h"

/* --- LVGL & UI --- */
#include "LVGL_Driver.h"
#include "LVGL_Example.h"
#include "LVGL_UI/LVGL_CounterPage.h"

/* --- App modules --- */
#include "counter.h"
#include "Wireless.h"
#include "wifi.h"
#include "Compass.h"

#include "System.h"

#ifdef __cplusplus
} /* extern "C" */
#endif
