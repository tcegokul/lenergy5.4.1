#include "LVGL_CounterPage.h"
#include <stdio.h>

#include "lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "assets/Qiyam.h"
#include "assets/ruku.h"
#include "assets/sujood.h"
#include "assets/juloos.h"

#include "assets/time_icon.h"
#include "assets/battery_icon.h"  

// #include "assets/random_icon.h"  // Battery icon if needed

static lv_obj_t *label_rakah;
static lv_obj_t *label_time;
static lv_obj_t *img_battery;
static lv_obj_t *img_posture_icon; // The single posture icon below rakah

#define MAX_RAKAH 4
static lv_obj_t *rakah_blocks[MAX_RAKAH];

// typedef enum {
//     STATE_QIYAM,
//     STATE_RUKU,
//     STATE_SUJOOD,
//     STATE_JULOOS
// } prayer_state_t;

void create_rakah_progress(lv_obj_t *parent)
{
    for (int i = 0; i < MAX_RAKAH; i++)
    {
        rakah_blocks[i] = lv_obj_create(parent);
        lv_obj_set_size(rakah_blocks[i], 15, 15);
        lv_obj_set_style_radius(rakah_blocks[i], 3, 0);
        lv_obj_set_style_border_width(rakah_blocks[i], 0, 0);
        lv_obj_set_style_bg_color(rakah_blocks[i], lv_color_hex(0x444444), 0); 
        lv_obj_align(rakah_blocks[i], LV_ALIGN_BOTTOM_MID, (i - 1.5) * 20, -10);
    }
}

void update_rakah_blocks(uint8_t completed_rakah)
{
    for (int i = 0; i < MAX_RAKAH; i++)
    {
        if (i < completed_rakah)
            lv_obj_set_style_bg_color(rakah_blocks[i], lv_color_white(), 0);
        else
            lv_obj_set_style_bg_color(rakah_blocks[i], lv_color_hex(0x444444), 0);
    }
}

void counter_page_create(lv_obj_t *parent)
{
    lv_obj_set_style_bg_color(parent, lv_color_black(), LV_PART_MAIN);

    // Smaller Time Label
    // label_time = lv_label_create(parent);
    // lv_label_set_text(label_time, "04:32");
    // lv_obj_set_style_text_color(label_time, lv_color_white(), 0);
    // lv_obj_set_style_text_font(label_time, &lv_font_montserrat_12, 0);
    // lv_obj_align(label_time, LV_ALIGN_TOP_LEFT, 5, 5);

//     img_time = lv_img_create(parent);
// lv_img_set_src(img_time, &time_icon);  // Set your time image source
// lv_obj_align(img_time, LV_ALIGN_TOP_LEFT, 5, 5);
// lv_obj_set_size(img_time, 15, 15);  // Adjust as needed

    // Battery Icon
    // img_battery = lv_img_create(parent);
    // lv_img_set_src(img_battery, &battery_icon);
    // lv_obj_align(img_battery, LV_ALIGN_TOP_RIGHT, -5, 5);
    // lv_obj_set_size(img_battery, 15, 10); 

    img_battery = lv_label_create(parent);
lv_label_set_text(img_battery, LV_SYMBOL_BATTERY_FULL);
lv_obj_set_style_text_color(img_battery, lv_color_white(), 0);
lv_obj_set_style_text_font(img_battery, &lv_font_montserrat_28, 0); // Adjust size as needed
lv_obj_align(img_battery, LV_ALIGN_TOP_RIGHT, -5, 5);
     

    // Rak'ah Label
    label_rakah = lv_label_create(parent);
    lv_label_set_text(label_rakah, "rak'ah 0");
    lv_obj_set_style_text_color(label_rakah, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_rakah, &lv_font_montserrat_28, 0);
    lv_obj_align(label_rakah, LV_ALIGN_CENTER, 0, -100); // Center higher to give space for icon
   

    // Posture Icon Below Rak'ah
    img_posture_icon = lv_img_create(parent);
    lv_img_set_src(img_posture_icon, &Qiyam);  // Default state
    lv_obj_align(img_posture_icon, LV_ALIGN_CENTER, 0, 50); 

    // Rakah Progress Blocks
    create_rakah_progress(parent);
}

void update_prayer_ui(uint8_t rakah, prayer_state_t state)
{
    // Update Rak'ah count text
    char buf[16];
    snprintf(buf, sizeof(buf), "rak'ah %d", rakah);
    lv_label_set_text(label_rakah, buf);

    // Update Rak'ah blocks
    update_rakah_blocks(rakah);

    // Update Posture Icon based on state
    switch (state)
    {
    case STATE_QIYAM:
        lv_img_set_src(img_posture_icon, &Qiyam);
        break;
    case STATE_RUKU:
        lv_img_set_src(img_posture_icon, &ruku);
        break;
    case STATE_SUJOOD_1:
    lv_img_set_src(img_posture_icon, &sujood);
        break;
    case STATE_SUJOOD_2:
        lv_img_set_src(img_posture_icon, &sujood);
        break;
    case STATE_JULLOS:
        lv_img_set_src(img_posture_icon, &juloos);
        break;
    default:
        break;
    }
}
