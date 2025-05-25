// vl53l0x_sensor.h
#ifndef VL53L0X_SENSOR_H
#define VL53L0X_SENSOR_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_RAKAHS 4

void vl53l0x_init();
void vl53l0x_update();
bool vl53l0x_was_new_sujood();
//static uint8_t last_status = 0;

uint8_t vl53l0x_get_last_status(void); //{
  //  return last_status;
//}


void rakah_set_prayer_type(int rakah_count);
void rakah_reset();
int rakah_get_current();
bool rakah_is_complete();
bool rakah_sujood1_done();
bool rakah_sujood2_done();

bool vl53l0x_sujood_detected(void);  // ✅ dans .h uniquement

#endif // VL53L0X_SENSOR_H


