
// // qibla_display.c
// #include "qibla_display.h"
// #include "color.h"
// #include "st7701s_display.h"


// #include "gps_parser.h"
// #include "nvs_flash.h"
// #include "nvs.h"
// #include <math.h>
// #include <stdbool.h>
// #include <stdio.h>

// #define DEG2RAD(x) ((x) * M_PI / 180.0f)
// #define RAD2DEG(x) ((x) * 180.0f / M_PI)

// #ifdef __cplusplus
// extern "C" {
// #endif


// static float qibla_angle = 0.0f;
// static bool fix_available = false;

// static void save_last_position(float lat, float lon) {
//     nvs_handle_t nvs;
//     if (nvs_open("qibla", NVS_READWRITE, &nvs) == ESP_OK) {
//         nvs_set_blob(nvs, "last_coords", (float[]){lat, lon}, sizeof(float) * 2);
//         nvs_commit(nvs);
//         nvs_close(nvs);
//     }
// }

// static bool load_last_position(float *lat, float *lon) {
//     nvs_handle_t nvs;
//     size_t len = sizeof(float) * 2;
//     float coords[2];
//     if (nvs_open("qibla", NVS_READONLY, &nvs) == ESP_OK) {
//         if (nvs_get_blob(nvs, "last_coords", coords, &len) == ESP_OK) {
//             *lat = coords[0];
//             *lon = coords[1];
//             nvs_close(nvs);
//             return true;
//         }
//         nvs_close(nvs);
//     }
//     return false;
// }

// static float calculate_qibla_angle(float lat1, float lon1) {
//     float lat2 = DEG2RAD(21.4225f); // Kaaba
//     float lon2 = DEG2RAD(39.8262f);
//     float dLon = DEG2RAD(lon2 - lon1);
//     float y = sin(dLon) * cos(lat2);
//     float x = cos(DEG2RAD(lat1)) * sin(lat2) - sin(DEG2RAD(lat1)) * cos(lat2) * cos(dLon);
//     float bearing = atan2f(y, x);
//     return fmodf(RAD2DEG(bearing) + 360.0f, 360.0f);
// }

// void qibla_display_init() {
//     float lat = 0, lon = 0;
//     if (gps_has_fix()) {
//         lat = gps_get_latitude();
//         lon = gps_get_longitude();
//         fix_available = true;
//         save_last_position(lat, lon);
//     } else if (load_last_position(&lat, &lon)) {
//         fix_available = false;
//     }
//     qibla_angle = calculate_qibla_angle(lat, lon);
// }

// void qibla_display_update(float heading_deg) {
//     // Affichage dans st7701s_display sera fait plus tard
//     float delta = qibla_angle - heading_deg;
//     if (delta < 0) delta += 360;
//     if (delta > 360) delta -= 360;
//     // TODO: stocker angle pour usage futur
// }

// float qibla_display_get_qibla_angle() {
//     return qibla_angle;
// }

// bool qibla_display_has_fix() {
//     return fix_available;
// }

// #ifdef __cplusplus
// }
// #endif