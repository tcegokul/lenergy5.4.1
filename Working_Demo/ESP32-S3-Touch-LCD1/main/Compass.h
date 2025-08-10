#ifndef COMPASS_H
#define COMPASS_H

#include "lvgl.h"   // LVGL core header
extern lv_obj_t *compass_cont;


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create the compass UI inside a given parent object.
 * 
 * @param parent       LVGL parent object (usually lv_scr_act()).
 * @param diameter_px  Compass diameter in pixels.
 */
void compass_ui_create(lv_obj_t *parent, int diameter_px);

/**
 * @brief Set the needle to a given bearing (degrees from True North).
 * 
 * @param bearing_deg  Angle 0..360°, clockwise from North.
 */
void compass_set_bearing_deg(double bearing_deg);



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // COMPASS_H
