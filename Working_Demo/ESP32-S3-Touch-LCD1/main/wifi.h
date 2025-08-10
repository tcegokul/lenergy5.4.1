#pragma once
#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---- Wi-Fi connect ----
bool connect_to_saved_wifi(void);
void wifi_init_softap(void);

// ---- Wi-Fi portal ----
void start_http_server(void);

// ---- Geolocation ----
double http_get_geolocation(void);
void parse_geolocation_response(const char *json_str);


#ifdef __cplusplus
}
#endif

static const char *TAG = "wifi_prov";
