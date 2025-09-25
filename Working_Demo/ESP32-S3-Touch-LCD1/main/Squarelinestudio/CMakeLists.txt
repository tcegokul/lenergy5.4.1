SET(SOURCES ui_comp_image4.c
    ui_comp.c
    ui_Screen1.c
    ui_Screen2.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_compass_needle_240x360_indexed16_png.c
    ui_img_wifi_icon_ring_white_on_black_8bit_png.c
    ui_img_battery_icon_half_white_on_black_8bit_png.c)

add_library(ui ${SOURCES})
