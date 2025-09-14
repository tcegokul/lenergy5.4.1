#pragma once
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SCR_LOC = 0,
    SCR_COUNTER,
} screen_id_t;

void screens_init(lv_obj_t *loc_scr, lv_obj_t *counter_scr);
lv_obj_t *screens_get(screen_id_t id);
void screens_switch(screen_id_t next, bool animated);

#ifdef __cplusplus
} /* extern "C" */
#endif