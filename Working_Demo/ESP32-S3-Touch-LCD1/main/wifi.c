// wifi.c
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_netif.h"

#include "esp_http_server.h"
#include "esp_http_client.h"

#include "cJSON.h"
#include "wifi.h"   // your header with the 5 public prototypes

// -------- Private defs (local to this file) --------
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define SOFTAP_SSID     "AL-IMAN-SETUP"
#define SOFTAP_CHANNEL  1
#define MAX_STA_CONN    4

#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1


static EventGroupHandle_t wifi_event_group;

static const char *wifi_form_html =
    "<!DOCTYPE html>"
    "<html><head><title>Wi-Fi Provisioning</title></head>"
    "<body><h2>Enter Wi-Fi Credentials</h2>"
    "<form action=\"/connect\" method=\"post\">"
    "SSID: <input type=\"text\" name=\"ssid\"><br><br>"
    "Password: <input type=\"password\" name=\"password\"><br><br>"
    "<input type=\"submit\" value=\"Connect\">"
    "</form></body></html>";

// -------- Private helpers (not exposed) --------
static esp_err_t root_get_handler(httpd_req_t *req)
{
    httpd_resp_send(req, wifi_form_html, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t connect_post_handler(httpd_req_t *req)
{
    char buf[200];
    int ret = httpd_req_recv(req, buf, MIN(req->content_len, sizeof(buf) - 1));
    if (ret <= 0) return ESP_FAIL;
    buf[ret] = '\0';

    char ssid[32] = {0}, password[64] = {0};
    sscanf(buf, "ssid=%31[^&]&password=%63s", ssid, password);

    ESP_LOGI(TAG, "Received SSID: %s, Password: %s", ssid, password);

    nvs_handle_t nvs;
    ESP_ERROR_CHECK(nvs_open("wifi_creds", NVS_READWRITE, &nvs));
    ESP_ERROR_CHECK(nvs_set_str(nvs, "ssid", ssid));
    ESP_ERROR_CHECK(nvs_set_str(nvs, "password", password));
    ESP_ERROR_CHECK(nvs_commit(nvs));
    nvs_close(nvs);

    httpd_resp_send(req, "<html><body><h3>Saved. Restarting...</h3></body></html>", HTTPD_RESP_USE_STRLEN);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    esp_restart();
    return ESP_OK;
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "STA started, connecting...");
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGW(TAG, "Failed to connect. Retrying...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Connected! IP Address: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

// -------- Public API (called from main.c) --------
bool connect_to_saved_wifi(void)
{
    char ssid[32] = {0}, password[64] = {0};
    nvs_handle_t nvs;
    if (nvs_open("wifi_creds", NVS_READONLY, &nvs) != ESP_OK)
        return false;

    size_t ssid_len = sizeof(ssid), pass_len = sizeof(password);
    if (nvs_get_str(nvs, "ssid", ssid, &ssid_len) != ESP_OK ||
        nvs_get_str(nvs, "password", password, &pass_len) != ESP_OK) {
        nvs_close(nvs);
        return false;
    }
    nvs_close(nvs);

    ESP_LOGI(TAG, "Connecting to saved Wi-Fi: %s", ssid);

    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_event_group = xEventGroupCreate();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);

    wifi_config_t wifi_config = {0};
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
                                           WIFI_CONNECTED_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           pdMS_TO_TICKS(15000));
    return (bits & WIFI_CONNECTED_BIT) != 0;
}

void wifi_init_softap(void)
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = SOFTAP_SSID,
            .ssid_len = strlen(SOFTAP_SSID),
            .channel = SOFTAP_CHANNEL,
            .password = "",
            .max_connection = MAX_STA_CONN,
            .authmode = WIFI_AUTH_OPEN
        },
    };

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "SoftAP started. SSID: %s", SOFTAP_SSID);
}

void start_http_server(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;

    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &(httpd_uri_t){ .uri="/", .method=HTTP_GET,  .handler=root_get_handler });
        httpd_register_uri_handler(server, &(httpd_uri_t){ .uri="/connect", .method=HTTP_POST, .handler=connect_post_handler });
        ESP_LOGI(TAG, "HTTP server started. Go to http://192.168.4.1");
    } else {
        ESP_LOGE(TAG, "Failed to start HTTP server");
    }
}

void parse_geolocation_response(const char *json_str)
{
    cJSON *root = cJSON_Parse(json_str);
    if (!root) { ESP_LOGE(TAG, "Failed to parse JSON"); return; }

    cJSON *lat = cJSON_GetObjectItem(root, "lat");
    cJSON *lon = cJSON_GetObjectItem(root, "lon");

    if (cJSON_IsNumber(lat) && cJSON_IsNumber(lon)) {
        ESP_LOGI(TAG, "Parsed Latitude: %f", lat->valuedouble);
        ESP_LOGI(TAG, "Parsed Longitude: %f", lon->valuedouble);
    } else {
        ESP_LOGW(TAG, "Latitude or Longitude not found in JSON");
    }
    cJSON_Delete(root);
}

void http_get_geolocation(void)
{
    esp_http_client_config_t config = {
        .url = "http://ip-api.com/json/",
        .method = HTTP_METHOD_GET,
        .timeout_ms = 5000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) { ESP_LOGE(TAG, "Failed to init HTTP client"); return; }

    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return;
    }

    int64_t content_length = esp_http_client_fetch_headers(client);
    int status = esp_http_client_get_status_code(client);
    ESP_LOGI(TAG, "HTTP Status: %d, Content-Length: %lld", status, (long long)content_length);
    if (status != 200) { esp_http_client_close(client); esp_http_client_cleanup(client); return; }

    const int CHUNK_SIZE = 256;
    char buffer[CHUNK_SIZE + 1];
    int total_read = 0;
    char json_accumulator[1024] = {0};

    while (1) {
        int read_len = esp_http_client_read(client, buffer, CHUNK_SIZE);
        if (read_len > 0) {
            buffer[read_len] = '\0';
            if ((total_read + read_len) < (int)sizeof(json_accumulator)) {
                memcpy(json_accumulator + total_read, buffer, read_len);
                total_read += read_len;
            }
        } else if (read_len == 0) break;
        else { ESP_LOGE(TAG, "Read error"); break; }
    }

    json_accumulator[total_read] = '\0';
    ESP_LOGI(TAG, "Full JSON: %s", json_accumulator);
    parse_geolocation_response(json_accumulator);

    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}
