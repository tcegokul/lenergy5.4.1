#include "screen_manager.h"

static lv_obj_t *s_loc_scr     = NULL;
static lv_obj_t *s_counter_scr = NULL;
static lv_obj_t *s_active      = NULL;

void screens_init(lv_obj_t *loc_scr, lv_obj_t *counter_scr)
{
    s_loc_scr     = loc_scr;
    s_counter_scr = counter_scr;
    s_active      = NULL;
}

lv_obj_t *screens_get(screen_id_t id)
{
    switch (id) {
        case SCR_LOC:     return s_loc_scr;
        case SCR_COUNTER: return s_counter_scr;
        default:          return NULL;
    }
}

void screens_switch(screen_id_t next, bool animated)
{
    lv_obj_t *target = screens_get(next);
    if (!target || target == s_active) return;

    if (animated) {
        lv_scr_load_anim(target, LV_SCR_LOAD_ANIM_MOVE_LEFT, 250, 0, false);
    } else {
        lv_scr_load(target);
    }
    s_active = target;
}
