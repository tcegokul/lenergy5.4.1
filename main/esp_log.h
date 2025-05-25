#ifndef CUSTOM_ESP_LOG_H
#define CUSTOM_ESP_LOG_H

#include "esp_log.h"  // Utilise le système natif ESP-IDF

#ifdef __cplusplus
extern "C" {
#endif

// Tu peux ajuster le niveau de log ici si besoin :
#define LOG_LEVEL ESP_LOG_INFO

// Macros optionnelles si tu veux filtrer selon le niveau :
#define LOG_ERROR(tag, fmt, ...) if (LOG_LEVEL >= ESP_LOG_ERROR) ESP_LOGE(tag, fmt, ##__VA_ARGS__)
#define LOG_WARN(tag, fmt, ...)  if (LOG_LEVEL >= ESP_LOG_WARN)  ESP_LOGW(tag, fmt, ##__VA_ARGS__)
#define LOG_INFO(tag, fmt, ...)  if (LOG_LEVEL >= ESP_LOG_INFO)  ESP_LOGI(tag, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(tag, fmt, ...) if (LOG_LEVEL >= ESP_LOG_DEBUG) ESP_LOGD(tag, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // CUSTOM_ESP_LOG_H
