#ifndef LVGL_COUNTERPAGE_H
#define LVGL_COUNTERPAGE_H
#pragma once
#include "lvgl.h"

//void counter_page_create(lv_obj_t *parent);

typedef enum {
    STATE_QIYAM,
    STATE_RUKU,
    STATE_SUJOOD_1,
    STATE_JULLOS,
    STATE_SUJOOD_2
} prayer_state_t;

void counter_page_create(lv_obj_t *parent);
void update_prayer_ui(uint8_t rakah, prayer_state_t state);
void show_start_screen(void);
#endif // LVGL_COUNTERPAGE_H
