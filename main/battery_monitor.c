
#include "battery_monitor.h"
#include "icons.h"  // pour les icônes comme icon_battery_25, etc.
#include "driver/gpio.h"

#define GPIO_CHARGE_DETECT GPIO_NUM_6

#ifdef __cplusplus
extern "C" {
#endif


static gpio_num_t charge_gpio_pin = GPIO_NUM_NC;

void battery_monitor_init(adc_channel_t channel, gpio_num_t usb_gpio)
{
    charge_gpio_pin = usb_gpio;

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << usb_gpio),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
}

bool battery_is_charging()
{
    return gpio_get_level(charge_gpio_pin) == 1;
}

// ✅ Fonction attendue par d’autres fichiers
bool battery_monitor_is_charging()
{
    return battery_is_charging();
}

int battery_get_level_percent() {
    // 👉 À adapter selon ta lecture ADC réelle
    // Ici : valeur simulée de 50% pour ne pas planter à la compilation
    return 50;
}


const uint16_t* battery_get_icon() {
    int level = battery_get_level_percent();

    if (battery_monitor_is_charging()) {
        return icon_battery_charge;
    } else if (level > 75) {
        return icon_battery_100;
    } else if (level > 50) {
        return icon_battery_75;
    } else if (level > 25) {
        return icon_battery_50;
    } else if (level > 10) {
        return icon_battery_25;
    } else {
        return icon_battery_empty;
    }
}

#ifdef __cplusplus
}
#endif