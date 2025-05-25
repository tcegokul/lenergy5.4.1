// espnow_sync.c
#include "espnow_sync.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_now.h"
#include "nvs_flash.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif



void init_wifi_as_sta(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

static espnow_status_t last_peer_status;

static void espnow_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
    if (len == sizeof(espnow_status_t)) {
        memcpy(&last_peer_status, data, sizeof(espnow_status_t));
        // Tu peux appeler display_peer_status() ici si tu veux afficher à l’écran
    }
}

void espnow_sync_init() {
    ESP_ERROR_CHECK(nvs_flash_init());
    init_wifi_as_sta();
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_recv_cb(espnow_recv_cb));
    // Aucun log ici, système silencieux
}

void espnow_sync_send_status(espnow_status_t *status) {
    uint8_t broadcast_addr[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    esp_now_peer_info_t peer = {0};
    memcpy(peer.peer_addr, broadcast_addr, 6);
    peer.channel = 0;
    peer.ifidx = ESP_IF_WIFI_STA;
    peer.encrypt = false;

    esp_now_add_peer(&peer);
    esp_now_send(broadcast_addr, (uint8_t *)status, sizeof(espnow_status_t));
    // Aucun message, mais envoi silencieux
}

espnow_status_t espnow_sync_get_last_peer_status() {
    return last_peer_status;
}

#ifdef __cplusplus
}
#endif