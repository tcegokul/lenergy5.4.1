#include "input_manager.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include <stdbool.h>

#define GPIO_BTN_UP     47
#define GPIO_BTN_BACK   0
#define GPIO_BTN_SELECT 21

#define DEBOUNCE_TIME_MS 200
#define LONG_PRESS_TIME_MS 1000

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    gpio_num_t gpio;
    button_event_t short_event;
    button_event_t long_event;
    int last_level;
    int64_t press_start_time;
    bool is_pressed;
    bool long_event_sent;
} button_t;

static button_t buttons[] = {
    { GPIO_BTN_BACK,   BUTTON_UP,     BUTTON_UP_LONG,     1, 0, false, false },
    { GPIO_BTN_UP,     BUTTON_DOWN,   BUTTON_DOWN_LONG,   1, 0, false, false },
    { GPIO_BTN_SELECT, BUTTON_SELECT, BUTTON_SELECT_LONG, 1, 0, false, false },
};

#define NUM_BUTTONS (sizeof(buttons) / sizeof(buttons[0]))

void input_manager_init(void) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        gpio_config_t io_conf = {
            .pin_bit_mask = 1ULL << buttons[i].gpio,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE
        };
        gpio_config(&io_conf);
    }
}

button_event_t input_manager_get_event(void) {
    int64_t now = esp_timer_get_time() / 1000;

    for (int i = 0; i < NUM_BUTTONS; i++) {
        int level = gpio_get_level(buttons[i].gpio);

        // Détection appui
        if (level == 0 && buttons[i].last_level == 1) {
            buttons[i].press_start_time = now;
            buttons[i].is_pressed = true;
            buttons[i].long_event_sent = false;
        }

        // Détection relâchement
        if (level == 1 && buttons[i].last_level == 0) {
            if (buttons[i].is_pressed) {
                buttons[i].is_pressed = false;
                if (!buttons[i].long_event_sent && (now - buttons[i].press_start_time) > DEBOUNCE_TIME_MS) {
                    return buttons[i].short_event;
                }
            }
        }

        // Appui long en cours
        if (level == 0 && buttons[i].is_pressed && !buttons[i].long_event_sent) {
            if ((now - buttons[i].press_start_time) >= LONG_PRESS_TIME_MS) {
                buttons[i].long_event_sent = true;
                return buttons[i].long_event;
            }
        }

        buttons[i].last_level = level;
    }

    return BUTTON_NONE;
}

#ifdef __cplusplus
}
#endif