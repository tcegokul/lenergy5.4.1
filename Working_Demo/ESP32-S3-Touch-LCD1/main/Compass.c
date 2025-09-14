#include "Compass.h"
//#include "compass_assets.h"  // your assets header
extern const lv_img_dsc_t ring_glow_420;   // your PNG (with alpha), any size

static lv_obj_t *s_img_ring = NULL;

// ---- Assets generated via LVGL Image Converter ----
// Place your converted C descriptors in a common header, e.g. assets_images.h
// and include it here. For clarity, we declare them extern:
extern const lv_img_dsc_t bg_ring_N_480x480;   // background + ring + "N"
extern const lv_img_dsc_t arrow_96x240;        // transparent needle
#define COL_CITY_YELLOW 0xFFD700  // gold/yellow
// ---- Optional fonts (if you converted custom fonts) ----
// extern const lv_font_t ui_font_Mont_20;
// extern const lv_font_t ui_font_Mont_48;

// ---- Persistent UI handles for this screen ----
static lv_obj_t *s_root = NULL;
static lv_obj_t *s_img_bg = NULL;
static lv_obj_t *s_img_needle = NULL;
static lv_obj_t *s_lbl_status = NULL;
static lv_obj_t *s_lbl_city = NULL;

// ---- Simple colors (or centralize in colors.h) ----
#define COL_BG_DARK    0x0B2430
#define COL_TEXT       0xE9F1E8

// ---- Layout constants (tune once) ----
#define SCR_W                480
#define SCR_H                480
#define COMPASS_SIZE         420
#define BOTTOM_TEXT_ZONE_H   140   // reserved space for labels

// Needle image is 96x240; pivot at tail (slightly above bottom)
#define NEEDLE_W             96
#define NEEDLE_H             240
#define NEEDLE_PIVOT_X       (NEEDLE_W/2)
#define NEEDLE_PIVOT_Y       (NEEDLE_H - 8)   // nudge to the tail

lv_obj_t*loc_screen_create(const char *city_initial)
{
     /* ---------- Root screen ---------- */
    s_root = lv_obj_create(NULL);
    lv_obj_clear_flag(s_root, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(s_root, SCR_W, SCR_H);
    lv_obj_set_style_bg_color(s_root, lv_color_hex(COL_BG_DARK), 0);
    lv_obj_set_style_bg_opa(s_root, LV_OPA_COVER, 0);

    /* ---------- Compass Ring Image ---------- */
    s_img_ring = lv_img_create(s_root);
    lv_img_set_src(s_img_ring, &ring_glow_420);
    lv_obj_align(s_img_ring, LV_ALIGN_CENTER, 0, -60);  // adjust as needed
    lv_obj_move_background(s_img_ring);

    /* ---------- Needle (transparent PNG) ---------- */
    s_img_needle = lv_img_create(s_root);
    // lv_img_set_src(s_img_needle, &arrow_96x240);
    lv_obj_center(s_img_needle);
    lv_img_set_pivot(s_img_needle, NEEDLE_PIVOT_X, NEEDLE_PIVOT_Y);
    lv_img_set_angle(s_img_needle, 0);  // 0° = pointing up

    /* ---------- Status label ---------- */
    s_lbl_status = lv_label_create(s_root);
    lv_label_set_text(s_lbl_status, "Localisation en cours...");
    lv_obj_set_style_text_color(s_lbl_status, lv_color_hex(COL_TEXT), 0);
    lv_obj_set_style_text_align(s_lbl_status, LV_TEXT_ALIGN_CENTER, 0);
    #if LV_FONT_MONTSERRAT_14
      lv_obj_set_style_text_font(s_lbl_status, &lv_font_montserrat_14, 0);
    #endif
    lv_obj_align(s_lbl_status, LV_ALIGN_CENTER, 0, 180);

    /* ---------- City label ---------- */
    s_lbl_city = lv_label_create(s_root);
    const char *city = (city_initial && city_initial[0]) ? city_initial : "LYON";
    lv_label_set_text(s_lbl_city, city);
    lv_obj_set_style_text_color(s_lbl_city, lv_color_hex(0xFFD700), 0);
    lv_obj_set_style_text_align(s_lbl_city, LV_TEXT_ALIGN_CENTER, 0);
    #if LV_FONT_MONTSERRAT_28
      lv_obj_set_style_text_font(s_lbl_city, &lv_font_montserrat_28, 0);
    #endif
    lv_obj_align_to(s_lbl_city, s_lbl_status, LV_ALIGN_OUT_BOTTOM_MID, 0, 16);

    return s_root;
}

void ui_show_loc_screen(const char *city_initial)
{
    if (!s_root) loc_screen_create(city_initial);
    lv_scr_load_anim(s_root, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, false);
}

void loc_screen_lock(float bearing_deg, const char *city)
{
    if (!s_root) return;

    // Rotate needle once; LVGL angle is in 0.1 degrees
    int16_t angle10 = (int16_t)(bearing_deg * 10.0f);
    lv_img_set_angle(s_img_needle, angle10);

    if (city && city[0] != '\0') {
        lv_label_set_text(s_lbl_city, city);
    }

    // No timers/animations here -> screen stays static
}

void loc_screen_destroy(void)
{
    if (!s_root) return;

    // If this is the active screen, load a blank before deleting
    if (lv_scr_act() == s_root) {
        lv_obj_t *blank = lv_obj_create(NULL);
        lv_scr_load(blank);
    }

    lv_obj_del(s_root);
    s_root = s_img_bg = s_img_needle = s_lbl_status = s_lbl_city = NULL;
}
