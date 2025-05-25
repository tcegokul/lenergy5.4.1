#ifndef ESP_CHECK_H
#define ESP_CHECK_H

#include <stdlib.h>
#include "esp_err.h"

/**
 * Vérifie une erreur sans log.
 */
#define ESP_ERROR_CHECK(x)         \
    do {                           \
        if ((x) != ESP_OK) {       \
            abort();               \
        }                          \
    } while (0)

#endif // ESP_CHECK_H
