#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "esp_log.h"

#include "gps_parser.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"


#define GPS_BUF_SIZE 1024

#ifdef __cplusplus
extern "C" {
#endif


static float latitude = 0.0f;
static float longitude = 0.0f;
static bool has_fix = false;
static char local_time[9] = "--:--";
static int timezone_offset = 0; // Ex: +1 pour Paris

static uart_port_t gps_uart = UART_NUM_1;

void gps_set_timezone_offset(int offset) {
    timezone_offset = offset;
}

bool gps_has_fix() {
    return has_fix;
}

float gps_get_latitude() {
    return latitude;
}

float gps_get_longitude() {
    return longitude;
}

const char* gps_get_local_time() {
    return local_time;
}

static void parse_gprmc(char *nmea) {
    // Exemple trame : $GPRMC,hhmmss,A,latitude,N,longitude,E,...
    char *token = strtok(nmea, ",");
    int field = 0;
    char time_str[7] = {0};

    while (token != NULL) {
        switch (field) {
            case 1: // UTC time
                strncpy(time_str, token, 6);
                break;
            case 2: // Fix status
                has_fix = (token[0] == 'A');
                break;
            case 3: // Latitude
                latitude = atof(token) / 100.0f;
                break;
            case 5: // Longitude
                longitude = atof(token) / 100.0f;
                break;
        }
        token = strtok(NULL, ",");
        field++;
    }

    if (has_fix && strlen(time_str) == 6) {
        int hour = ((time_str[0]-'0')*10 + (time_str[1]-'0') + timezone_offset) % 24;
        int min = (time_str[2]-'0')*10 + (time_str[3]-'0');
        snprintf(local_time, sizeof(local_time), "%02d:%02d", hour % 24, min % 60);

    }
}

void gps_parser_task(void *arg) {
    uint8_t data[GPS_BUF_SIZE];
    char line[128];
    int line_pos = 0;

    while (1) {
        int len = uart_read_bytes(gps_uart, data, sizeof(data), pdMS_TO_TICKS(100));
        
        if (len < 0) {
            ESP_LOGE("GPS", "UART read error");
            continue;
        }
        if (len > 0) {
                 data[len] = '\0';  // Null-terminate the string
                 ESP_LOGI("GPS", "Raw: %s", (char *)data);
            }


        for (int i = 0; i < len; i++) {
            if (data[i] == '\n' || line_pos >= sizeof(line) - 1) {
                line[line_pos] = '\0';
                if (strstr(line, "$GPRMC")) {
                    parse_gprmc(line);
                }
                line_pos = 0;
            } else if (data[i] != '\r') {
                line[line_pos++] = data[i];
            }
        }
    }
    
}

void gps_parser_init(uart_port_t uart_num) {
    gps_uart = uart_num;
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(gps_uart, &uart_config);
    uart_driver_install(gps_uart, GPS_BUF_SIZE * 2, 0, 0, NULL, 0);
    xTaskCreate(gps_parser_task, "gps_task", 4096, NULL, 5, NULL);
    
}

const char* gps_get_location_name(void) {
    float lat = gps_get_latitude();
    float lon = gps_get_longitude();

    if (lat == 0.0f && lon == 0.0f) return "No Fix";

    // 🇲🇦 Maroc
    if (lat >= 27 && lat <= 36 && lon >= -17 && lon <= -1) {
        if (lat > 33) return "Rabat";
        if (lat > 31) return "Marrakech";
        return "Agadir";
    }

    // 🇩🇿 Algérie
    if (lat >= 28 && lat <= 37 && lon >= -9 && lon <= 9) {
        if (lat > 35) return "Alger";
        if (lat > 33) return "Oran";
        return "Ghardaïa";
    }

    // 🇹🇳 Tunisie
    if (lat >= 32 && lat <= 37 && lon >= 7 && lon <= 12) {
        if (lat > 35) return "Tunis";
        return "Sfax";
    }

    // 🇫🇷 France
    if (lat >= 42 && lat <= 51 && lon >= -5 && lon <= 8) {
        if (lat > 48) return "Lille";
        if (lat > 45) return "Paris";
        if (lat > 43) return "Lyon";
        return "Marseille";
    }

    // 🇮🇹 Italie
    if (lat >= 36 && lat <= 47 && lon >= 6 && lon <= 18) {
        if (lat > 45) return "Milan";
        if (lat > 41) return "Rome";
        return "Naples";
    }

    // 🌍 Moyen-Orient
    if (lat >= 22 && lat <= 40 && lon >= 30 && lon <= 60) {
        if (lat > 30 && lon > 44) return "Bagdad";
        if (lat > 24 && lon > 46) return "Riyad";
        if (lat > 25 && lon < 37) return "Le Caire";
        return "Dubaï";
    }

    // 🌎 Autres
    if (lat > 50 && lon < -50) return "Canada";
    if (lat < 0 && lon < -30) return "Brésil";
    if (lat < -20 && lon > 110) return "Australie";
    if (lat > 30 && lon > 100) return "Chine";
    if (lat > 20 && lon > 70 && lon < 90) return "Inde";

    return "World";
}

float gps_get_qibla_distance(void) {
    // Juste une valeur par défaut pour le moment
    return 2200.0f;  // Ex. distance de Paris à la Mecque
}

#ifdef __cplusplus
}
#endif