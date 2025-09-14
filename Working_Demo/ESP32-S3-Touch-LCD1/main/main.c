#include"defintions.h"
#include <time.h>
#include <sys/time.h>
#include "esp_sntp.h"
#include "screen_manager.h"
#include "Squarelinestudio/ui.h"

//lv_obj_t *compass_cont = NULL;


// ----- Timers & sensors -----
TimerHandle_t my_timer;
bool accel_Data_rdy = false;
uint8_t buf[6];

extern IMUdata Accel;
VL53L0X_RangingMeasurementData_t measurement1;
extern VL53L0X_Dev_t sensor;
extern lv_obj_t *led;

// ----- Prayer state tracking -----
prayer_state_t current_state  = STATE_QIYAM;
prayer_state_t previous_state = STATE_QIYAM;

uint16_t previous_distance = 2000;
uint8_t rakah_counter      = 0;
time_t last_print_epoch = 0;  

bool wifi_connected = false;


// ----- SNTP / Time (no UI) -----
static volatile bool g_time_synced = false;

static void sntp_sync_cb(struct timeval *tv)
{
    (void)tv;
    g_time_synced = true;
    ESP_LOGI("TIME", "SNTP sync complete");
}

static void time_start_sntp(void)
{
    // Set your local time zone.
    // IST example (UTC+5:30, no DST): "IST-5:30"
    // UTC: "UTC0"
    // Paris with DST: "CET-1CEST,M3.5.0/2,M10.5.0/3"
    setenv("TZ", "IST-5:30", 1);
    tzset();

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(sntp_sync_cb);
    sntp_init();
}

static bool time_is_valid(void)
{
    // After first sync, time() should be well past epoch.
    return time(NULL) > 100000;
}

static void print_current_time_uart(void)
{
    if (!time_is_valid()) return;

    time_t now = time(NULL);
    struct tm lt;
    localtime_r(&now, &lt);

    char buf[32];
    // 2025-08-15 14:23:07
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &lt);
    printf("TIME: %s\n", buf);
}

void process_prayer_state(uint16_t distance_mm, uint8_t status)
{
    ESP_LOGI("DEBUG", "Distance: %u mm, Status: %u", distance_mm, status);

    if (status != 0) {
        ESP_LOGW("SKIP", "Invalid measurement, skipping...");
        return;
    }

    prayer_state_t detected_posture = current_state; // Default to current unless valid change

    // Context-aware posture detection
    switch (current_state) {
        case STATE_QIYAM:
            if (distance_mm >= 500 && distance_mm < 1000)
                detected_posture = STATE_RUKU;
            break;

        case STATE_RUKU:
            if (distance_mm >= 50 && distance_mm < 150)
                detected_posture = STATE_SUJOOD_1;
            break;

        case STATE_SUJOOD_1:
            if (distance_mm == 0 || distance_mm >= 1000)
                detected_posture = STATE_JULLOS;
            break;

        case STATE_JULLOS:
            if (distance_mm >= 50 && distance_mm < 150)
                detected_posture = STATE_SUJOOD_2;
            break;

        case STATE_SUJOOD_2:
            if (distance_mm == 0 || distance_mm >= 1000)
                detected_posture = STATE_QIYAM;
            break;

        default:
            break;
    }

    // Always log what posture sensor sees
    const char* posture_names[] = {
        "Qiyam", "Ruku", "Sujood 1", "Jullos", "Sujood 2"
    };
    ESP_LOGI("POSTURE", "Sensor infers posture: %s", posture_names[detected_posture]);

    // Check transition
    if (detected_posture != current_state) {
        previous_state = current_state;
        current_state = detected_posture;

        ESP_LOGI("PRAYER", "Transitioned from %s to %s",
                 posture_names[previous_state],
                 posture_names[current_state]);

        // Increment rak‘ah after completing cycle
        if (current_state == STATE_SUJOOD_2 && previous_state == STATE_JULLOS) {
            rakah_counter++;
            ESP_LOGI("RAKAH", "Rak‘ah completed: %d", rakah_counter);
        }
    }
}
 

void my_timer_callback(TimerHandle_t xTimer)
{
    accel_Data_rdy = true;
}

uint16_t xshut_state = 0;

void start_timer()
{
    my_timer = xTimerCreate("MyTimer", pdMS_TO_TICKS(1000), pdTRUE, NULL, my_timer_callback);

    if (my_timer != NULL)
    {
        xTimerStart(my_timer, 0);
    } 
}

// One-shot LVGL timer to switch from Location -> Counter
static void switch_to_counter_cb(lv_timer_t *t)
{
    (void)t;
    screens_switch(SCR_COUNTER, true);   // animate to counter
    // Timer is one-shot; no need to delete explicitly if created with period>0 and not reused.
    // If you want to be explicit: lv_timer_del(t);
}


const char *city;
void app_main(void)
{ 

    system_initalise();
    
    //intialise timer
    start_timer();
    

    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_connected = connect_to_saved_wifi();

    if (wifi_connected) {
        
        ESP_LOGI(TAG, "STA connected.");
        time_start_sntp();
        double qibla_deg = http_get_geolocation();
        city = http_get_city();
        
        if (qibla_deg >= 0){

            printf("QIBLA: %.2f° from North\n", qibla_deg);
        }

        
        else{
            ESP_LOGI(TAG, "Starting Captive Portal Mode...");
            wifi_init_softap();
            start_http_server();
             
        }
    }
   

   //working part 

    // lv_obj_t *counter_screen = lv_obj_create(NULL);
    // counter_page_create(counter_screen);
    // lv_scr_load(counter_screen);

   
    //Squareline studio part
    ui_init();
    

    while (1) {


        vTaskDelay(pdMS_TO_TICKS(10));

    

        lv_timer_handler();

        if (accel_Data_rdy)
         {
            accel_Data_rdy = false;
            
           VL53L0X_PerformSingleRangingMeasurement(&sensor, &measurement1);
             uint16_t distance = measurement1.RangeMilliMeter;
             uint8_t status = measurement1.RangeStatus;


              
                //  process_prayer_state(distance, status);
                //  update_prayer_ui(rakah_counter, current_state);
            
            } 

           

   
    }

}
