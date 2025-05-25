// espnow_sync.h
#ifndef ESPNOW_SYNC_H
#define ESPNOW_SYNC_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t prayer_status;   // 0 = idle, 1 = active
    uint8_t battery_level;   // 0–100
    float latitude;
    float longitude;
} espnow_status_t;

void espnow_sync_init();
void espnow_sync_send_status(espnow_status_t *status);
espnow_status_t espnow_sync_get_last_peer_status();

#endif // ESPNOW_SYNC_H


