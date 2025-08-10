#include "Compass.h"
#include <math.h>
#include <stdio.h>
#include "lvgl.h"

// ------- Persistent UI elements -------
//static lv_obj_t *compass_cont = NULL;
static lv_obj_t *needle_line  = NULL;
static lv_obj_t *lbl_bearing  = NULL;
static lv_obj_t *compass_cont = NULL;
// NSEW labels (now explicitly kept)
static lv_obj_t *label_N = NULL;
static lv_obj_t *label_E = NULL;
static lv_obj_t *label_S = NULL;
static lv_obj_t *label_W = NULL;

// If you draw the needle as a line, keep points persistent
static lv_point_t g_pts[2] = {{0,0},{0,0}};

// Shared style for big font (init once)
static lv_style_t style_big_font;
static bool style_big_font_inited = false;

// ---------- API: set bigger font on NSEW ----------
static void compass_labels_set_big_font(void)
{
    if (!style_big_font_inited) {
        lv_style_init(&style_big_font);
        // Ensure LV_FONT_MONTSERRAT_28 (or 24) is enabled in lv_conf.h:
        // #define LV_FONT_MONTSERRAT_28 1
        lv_style_set_text_font(&style_big_font, &lv_font_montserrat_28);
        // Optional: slightly tighter letter spacing for small TFTs
        // lv_style_set_text_letter_space(&style_big_font, -1);
        style_big_font_inited = true;
    }

    if (label_N) lv_obj_add_style(label_N, &style_big_font, LV_PART_MAIN);
    if (label_E) lv_obj_add_style(label_E, &style_big_font, LV_PART_MAIN);
    if (label_S) lv_obj_add_style(label_S, &style_big_font, LV_PART_MAIN);
    if (label_W) lv_obj_add_style(label_W, &style_big_font, LV_PART_MAIN);
}


void compass_set_bearing_deg(double bearing_deg)
{
   if (!compass_cont || !needle_line) return;

    // Ensure we have real sizes (in case this runs right after creation)
    int w = lv_obj_get_width(compass_cont);
    int h = lv_obj_get_height(compass_cont);
    if (w == 0 || h == 0) {
        lv_obj_update_layout(compass_cont);
        w = lv_obj_get_width(compass_cont);
        h = lv_obj_get_height(compass_cont);
    }
    int diameter = LV_MIN(w, h);
    if (diameter <= 2) return;

    // Center of the dial
    const int cx = diameter / 2;
    const int cy = diameter / 2;

    // Margin: clamp so it never eats the whole radius
    int margin = lv_dpx(14);
    int max_safe = diameter / 5;                 // ~20% of diameter
    if (margin > max_safe) margin = max_safe;
    if (margin < 2) margin = 2;

    int radius = (diameter / 2) - margin;
    if (radius < 2) radius = 2;                  // keep visible length

    // Convert compass bearing (0°=North, CW+) -> math angle (0°=East, CCW+)
    float theta = (90.0f - bearing_deg) * (float)M_PI / 180.0f;

    // Tip point
    int x_tip = cx + (int)lrintf(radius * cosf(theta));
    int y_tip = cy - (int)lrintf(radius * sinf(theta));

    // Update points in the needle's local coordinate space
    g_pts[0].x = cx;   g_pts[0].y = cy;
    g_pts[1].x = x_tip; g_pts[1].y = y_tip;

    // Make the line object use the same coordinate space as the container
    lv_obj_set_size(needle_line, diameter, diameter);
    lv_obj_align(needle_line, LV_ALIGN_CENTER, 0, 0);

    lv_line_set_points(needle_line, g_pts, 2);
}


// ---------- Create once ----------
void compass_ui_create(lv_obj_t *parent, int diameter_px)
{
    // Container
    compass_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(compass_cont);
    lv_obj_set_size(compass_cont, diameter_px, diameter_px);
    lv_obj_center(compass_cont);
    lv_obj_clear_flag(compass_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(compass_cont, LV_RADIUS_CIRCLE, 0);

    // Dial (white bg + black border)
    lv_obj_set_style_bg_opa(compass_cont, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(compass_cont, lv_color_white(), 0);
    lv_obj_set_style_border_width(compass_cont, 10, 0);
    lv_obj_set_style_border_color(compass_cont, lv_color_black(), 0);
    lv_obj_set_style_shadow_width(compass_cont, 18, 0);

    lv_obj_move_foreground(compass_cont);

    // --- NSEW labels (now explicit handles) ---
    label_N = lv_label_create(compass_cont);
    lv_label_set_text(label_N, "N");
    lv_obj_set_style_text_color(label_N, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_align(label_N, LV_ALIGN_TOP_MID, 0, lv_dpx(6));

    label_E = lv_label_create(compass_cont);
    lv_label_set_text(label_E, "E");
    lv_obj_set_style_text_color(label_E, lv_color_black(), 0);
    lv_obj_align(label_E, LV_ALIGN_RIGHT_MID, -lv_dpx(6), 0);

    label_S = lv_label_create(compass_cont);
    lv_label_set_text(label_S, "S");
    lv_obj_set_style_text_color(label_S, lv_color_black(), 0);
    lv_obj_align(label_S, LV_ALIGN_BOTTOM_MID, 0, -lv_dpx(6));

    label_W = lv_label_create(compass_cont);
    lv_label_set_text(label_W, "W");
    lv_obj_set_style_text_color(label_W, lv_color_black(), 0);
    lv_obj_align(label_W, LV_ALIGN_LEFT_MID, lv_dpx(6), 0);

    // Apply bigger font to all four
    compass_labels_set_big_font();

    // Optional center cap
    lv_obj_t *cap = lv_obj_create(compass_cont);
    lv_obj_remove_style_all(cap);
    int cap_d = lv_dpx(10);
    lv_obj_set_size(cap, cap_d, cap_d);
    lv_obj_center(cap);
    lv_obj_set_style_radius(cap, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(cap, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(cap, lv_color_black(), 0);

  

    // Needle (line)
    needle_line = lv_line_create(compass_cont);
    lv_obj_set_size(needle_line, diameter_px, diameter_px);
    lv_obj_align(needle_line, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_line_width(needle_line, 8, 0);
    lv_obj_set_style_line_rounded(needle_line, true, 0);
    lv_obj_set_style_line_color(needle_line, lv_palette_main(LV_PALETTE_RED), 0);

    // Initialize points (optional; will be overwritten by setter)
    g_pts[0].x = diameter_px/2; g_pts[0].y = diameter_px/2;
    g_pts[1].x = diameter_px/2; g_pts[1].y = lv_dpx(10);
    lv_line_set_points(needle_line, g_pts, 2);

    //compass_set_bearing_deg(287.92f);
}

void compass_hide(void)
{
    if (compass_cont) lv_obj_add_flag(compass_cont, LV_OBJ_FLAG_HIDDEN);
}

void compass_show(void)
{
    if (compass_cont) lv_obj_clear_flag(compass_cont, LV_OBJ_FLAG_HIDDEN);
}