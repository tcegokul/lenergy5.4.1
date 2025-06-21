// vl53l0x_sensor.c

//#include "color.h"
//#include "st7701s_display.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "esp_timer.h"
#include "vl53l0x_api.h"
#include "vl53l0x_def.h"
#include "vl53l0x_sensor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

#define TAG "VL53L0X"
#define SUJOOD_THRESHOLD_MM 200
#define ANTI_REBOND_MS 1500

// Couleurs en format RGB565
#define COLOR_BLACK         0x0000
#define COLOR_WHITE         0xFFFF
#define COLOR_RED           0xF800
#define COLOR_GREEN         0x07E0
#define COLOR_BLUE          0x001F
#define COLOR_CYAN          0x07FF
#define COLOR_MAGENTA       0xF81F
#define COLOR_YELLOW        0xFFE0
#define COLOR_ORANGE        0xFD20
#define COLOR_PURPLE        0x8010
#define COLOR_GRAY          0x8410
#define COLOR_BROWN         0xA145
#define COLOR_GOLD          0xFEA0
#define COLOR_SKYBLUE       0x867D
#define COLOR_PINK          0xF81F

// Couleurs logiques pour ton projet
#define COLOR_TEXT_DEFAULT      COLOR_WHITE
#define COLOR_TEXT_HIGHLIGHT    COLOR_CYAN
#define COLOR_TEXT_ERROR        COLOR_RED
#define COLOR_TEXT_OK           COLOR_GREEN
#define COLOR_BACKGROUND_DAY    COLOR_BLACK
#define COLOR_BACKGROUND_NIGHT  COLOR_BLUE

#ifdef __cplusplus
extern "C" {
#endif



static int current_rakah = 0;
static int total_rakah = 2;
static bool in_sujood = false;
static bool sujood1 = false;
static bool sujood2 = false;
static bool prayer_complete = false;
static int64_t last_detection_time = 0;

typedef struct {
    bool sujood1_done;
    bool sujood2_done;
    bool complete;
} rakah_state_t;

static rakah_state_t rakahs[MAX_RAKAHS];

VL53L0X_Dev_t sensor = {.I2cDevAddr = 0x29};
static VL53L0X_RangingMeasurementData_t measurement;
static uint8_t last_range_status = 255;  // 255 = inconnu

uint32_t refSpadCount = 0;
uint8_t isApertureSpads = 0;


int8_t VLX_STATUS_RETURN = 0;

void vl53l0x_init() {
    memset(rakahs, 0, sizeof(rakahs));

    /* VLX_STATUS_RETURN = VL53L0X_WaitDeviceBooted(&sensor);
    if (VLX_STATUS_RETURN != VL53L0X_ERROR_NONE) {
        ESP_LOGI("vlx", "Sensor not booted: %d\n", VLX_STATUS_RETURN);
        return;
} */


    VLX_STATUS_RETURN = VL53L0X_DataInit(&sensor);
    VLX_STATUS_RETURN = VL53L0X_StaticInit(&sensor);
    VLX_STATUS_RETURN = VL53L0X_PerformRefCalibration(&sensor, NULL, NULL);
    VLX_STATUS_RETURN = VL53L0X_PerformRefSpadManagement(&sensor, &refSpadCount, &isApertureSpads);
    VLX_STATUS_RETURN = VL53L0X_SetDeviceMode(&sensor, VL53L0X_DEVICEMODE_SINGLE_RANGING);
    VLX_STATUS_RETURN = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(&sensor, 20000);
    
}


void rakah_set_prayer_type(int rakah_count) {
    total_rakah = rakah_count;
    rakah_reset();
}

void rakah_reset() {
    current_rakah = 0;
    sujood1 = sujood2 = in_sujood = prayer_complete = false;
    memset(rakahs, 0, sizeof(rakahs));
    last_detection_time = esp_timer_get_time() / 1000;
}

bool vl53l0x_is_prostrating() {
    VL53L0X_PerformSingleRangingMeasurement(&sensor, &measurement);
    //st7701s_display_distance_status(measurement.RangeStatus);

    // ✅ Stocke le statut pour l'affichage
    last_range_status = measurement.RangeStatus;

    return (measurement.RangeStatus == 0 && measurement.RangeMilliMeter < SUJOOD_THRESHOLD_MM);
}

bool vl53l0x_was_new_sujood() {
    return (sujood1 && !rakahs[current_rakah].sujood1_done) || (sujood2 && !rakahs[current_rakah].sujood2_done);
}

void vl53l0x_update() {
    if (prayer_complete) return;

    bool now_sujood = vl53l0x_is_prostrating();
    int64_t now = esp_timer_get_time() / 1000;

    if (now_sujood && !in_sujood && now - last_detection_time > ANTI_REBOND_MS) {
        in_sujood = true;
        last_detection_time = now;

        if (!rakahs[current_rakah].sujood1_done) {
            rakahs[current_rakah].sujood1_done = true;
            sujood1 = true;
            
        } else if (!rakahs[current_rakah].sujood2_done) {
            rakahs[current_rakah].sujood2_done = true;
            sujood2 = true;
            rakahs[current_rakah].complete = true;
            

            current_rakah++;
            if (current_rakah >= total_rakah) {
                prayer_complete = true;
               
            }
        }
    } else if (!now_sujood) {
        in_sujood = false;
        sujood1 = sujood2 = false;
    }
}

int rakah_get_current() {
    return current_rakah;
}

bool rakah_is_complete() {
    return prayer_complete;
}

bool rakah_sujood1_done() {
    return rakahs[current_rakah].sujood1_done;
}

bool rakah_sujood2_done() {
    return rakahs[current_rakah].sujood2_done;
}

bool vl53l0x_sujood_detected(void) {
    return vl53l0x_was_new_sujood();
}

uint8_t vl53l0x_get_last_status(void) {
    return last_range_status;
}

#ifdef __cplusplus
}
#endif


