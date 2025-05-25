#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <stdbool.h>
#include "driver/gpio.h"
#include "driver/adc.h"

void battery_monitor_init(adc_channel_t channel, gpio_num_t usb_gpio);
int battery_get_level_percent();
bool battery_monitor_is_charging();
const uint16_t* battery_get_icon();



#endif // BATTERY_MONITOR_H
