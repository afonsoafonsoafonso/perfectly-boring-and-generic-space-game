// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

int (get_mode_info)(uint16_t mode, vbe_mode_info_t *vmi_p);

int (vc_set_mode)(uint16_t mode);

void *(vc_vram_mapping)(uint16_t mode);

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

void *(vg_init)(uint16_t mode);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_draw_xpm)(const char **map, int x, int y);

int vbe_get_control_info();

