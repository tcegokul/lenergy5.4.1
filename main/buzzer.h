
// buzzer.h
#ifndef BUZZER_H
#define BUZZER_H

#include "driver/gpio.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void buzzer_init(gpio_num_t gpio);
void buzzer_beep();
void buzzer_play_completion();
void buzzer_play_alert();
void buzzer_set_enabled(bool enabled);
bool buzzer_is_enabled();

void buzzer_beep_sujood(void);  // ✅ pour bip sujood
void buzzer_beep_end(void);     // ✅ pour bip fin prière

#ifdef __cplusplus
}
#endif

#endif // BUZZER_H


