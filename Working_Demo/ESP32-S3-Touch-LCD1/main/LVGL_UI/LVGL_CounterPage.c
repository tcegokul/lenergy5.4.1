#include "LVGL_CounterPage.h"
#include <stdio.h>

#include "lvgl.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



#include "assets/Qiyam.h"   // Posture icons
#include "assets/ruku.h"
#include "assets/sujood.h"
#include "assets/juloos.h"

lv_obj_t *label_rakah;
lv_obj_t *label_state;
static lv_obj_t *img_posture_icon;
static lv_obj_t *start_screen;

void show_start_screen(void)
{
    start_screen = lv_obj_create(NULL); // Create a new screen
    lv_obj_set_style_bg_color(start_screen, lv_color_black(), LV_PART_MAIN);

    // Label: "Ready for prayer"
    lv_obj_t *label1 = lv_label_create(start_screen);
    lv_label_set_text(label1, "Ready for prayer");
    lv_obj_set_style_text_color(label1, lv_color_white(), 0);
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_16, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -20);

    // Label: "Calibrating the sensor..."
    lv_obj_t *label2 = lv_label_create(start_screen);
    lv_label_set_text(label2, "Calibrating the sensor...");
    lv_obj_set_style_text_color(label2, lv_color_white(), 0);
    lv_obj_set_style_text_font(label2, &lv_font_montserrat_14, 0);
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 20);

    lv_scr_load(start_screen);

    // // Create a timer to switch to main UI after 3 seconds
    // lv_timer_t *timer = lv_timer_create_delayed([](lv_timer_t *t){
    //     lv_obj_t *main_screen = lv_obj_create(NULL);
    //     counter_page_create(main_screen);
    //     lv_scr_load(main_screen);
    //     lv_timer_del(t);
    // }, 3000, NULL);

   vTaskDelay(pdMS_TO_TICKS(3000));
}

void counter_page_create(lv_obj_t *parent)
{
    // Set background to black
    lv_obj_set_style_bg_color(parent, lv_color_black(), LV_PART_MAIN);

    // Rak‘ah label
    label_rakah = lv_label_create(parent);
    lv_obj_align(label_rakah, LV_ALIGN_TOP_MID, 0, 10);
    lv_label_set_text(label_rakah, "Rak‘ah: 0");
   // lv_obj_set_style_text_font(label_rakah, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(label_rakah, lv_color_hex(0xFFFF99), 0);

    // Prayer state label
    label_state = lv_label_create(parent);
    lv_obj_align(label_state, LV_ALIGN_TOP_MID, 0, 50);
    lv_label_set_text(label_state, "State: Qiyam");
    //lv_obj_set_style_text_font(label_state, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(label_state, lv_color_hex(0xFFFF99), 0);

    // Posture icon image
    img_posture_icon = lv_img_create(parent);
    lv_obj_align(img_posture_icon, LV_ALIGN_CENTER, 0, 50);  // push icon slightly down
    //lv_obj_set_size(img_posture_icon, 96, 96);               // manually set icon size
    lv_img_set_src(img_posture_icon, &Qiyam); 
    //lv_img_set_zoom(img_posture_icon, 512);               // default
}

void update_prayer_ui(uint8_t rakah, prayer_state_t state)
{
    static const char* state_names[] = {
        "Qiyam", "Ruku", "Sujood 1", "Jullos", "Sujood 2"
    };

    char buf[32];

    snprintf(buf, sizeof(buf), "Rak‘ah: %d", rakah);
    lv_label_set_text(label_rakah, buf);

    snprintf(buf, sizeof(buf), "State: %s", state_names[state]);
    lv_label_set_text(label_state, buf);

    // Update icon
    switch (state) {
        case STATE_QIYAM:
            lv_img_set_src(img_posture_icon, &Qiyam);
            break;
        case STATE_RUKU:
            lv_img_set_src(img_posture_icon, &ruku);
            break;
        case STATE_SUJOOD_1:
        case STATE_SUJOOD_2:
            lv_img_set_src(img_posture_icon, &sujood);
            break;
        case STATE_JULLOS:
            lv_img_set_src(img_posture_icon, &juloos);
            break;
    }
}
