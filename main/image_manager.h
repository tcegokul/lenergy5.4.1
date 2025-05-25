// image_manager.h
#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    const uint16_t* data;
    const uint16_t* data_night;
    int width;
    int height;
} image_t;

void image_manager_init();
const image_t* image_manager_get_background(int index);
const image_t* image_manager_get_preview(int index);
int image_manager_get_total();

bool is_night_mode();

#endif // IMAGE_MANAGER_H

