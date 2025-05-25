
// buzzer.c
#include "buzzer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static gpio_num_t buzzer_gpio = GPIO_NUM_NC;
static bool buzzer_enabled = true;

#ifdef __cplusplus
extern "C" {
#endif


void buzzer_init(gpio_num_t gpio) {
    buzzer_gpio = gpio;
    gpio_set_direction(buzzer_gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(buzzer_gpio, 0);
}

void buzzer_set_enabled(bool enabled) {
    buzzer_enabled = enabled;
}

bool buzzer_is_enabled() {
    return buzzer_enabled;
}

static void buzzer_on(int duration_ms) {
    if (!buzzer_enabled) return;
    gpio_set_level(buzzer_gpio, 1);
    vTaskDelay(pdMS_TO_TICKS(duration_ms));
    gpio_set_level(buzzer_gpio, 0);
}

void buzzer_beep() {
    buzzer_on(100); // Bip court
}

void buzzer_play_completion() {
    buzzer_on(150);
    vTaskDelay(pdMS_TO_TICKS(100));
    buzzer_on(150);
    vTaskDelay(pdMS_TO_TICKS(100));
    buzzer_on(250);
}

void buzzer_play_alert() {
    for (int i = 0; i < 3; i++) {
        buzzer_on(300);
        vTaskDelay(pdMS_TO_TICKS(150));
    }
}

void buzzer_beep_sujood() {
    buzzer_on(200); // Bip long pour sujood
    // customer has to add logic here 
}
void buzzer_beep_end() {
    buzzer_on(500); // Bip long pour fin de prière
    //customer has to add logic here
}


#ifdef __cplusplus
}
#endif