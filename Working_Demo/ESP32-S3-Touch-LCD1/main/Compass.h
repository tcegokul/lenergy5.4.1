#pragma once
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

// Build the screen object tree (does NOT show it)
lv_obj_t* loc_screen_create(const char *city_initial); 

// Show this screen (loads the built screen)
void ui_show_loc_screen(const char *city_initial);

// One-shot: set final bearing and city, then leave it static
void loc_screen_lock(float bearing_deg, const char *city);

// Optional: remove/free everything (use if RAM is tight)
void loc_screen_destroy(void);

const char* http_get_city(void);

#ifdef __cplusplus
} /* extern "C" */
#endif
