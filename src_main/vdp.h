#ifndef VDP_H
#define VDP_H
#include <stdint.h>

#define VDP_DATA_PORT   (*(volatile uint16_t*)0xC00000)
#define VDP_CTRL_PORT   (*(volatile uint16_t*)0xC00004)
#define VDP_HV_COUNTER  (*(volatile uint16_t*)0xC00008)

#define PLANE_A_ADDR 0xC000
#define PLANE_B_ADDR 0xE000
#define SCROLL_A_PLANE 0xC000
#define CRAM_ADDR 0xC000

void vdp_init(void);
void vdp_clear(void);
void vdp_load_font(void);
void vdp_puts(int x, int y, const char *str);
void vdp_set_palette(uint16_t index, uint16_t color);
void vdp_upload_tiles(const uint32_t *data, uint16_t tile_index, uint16_t count);
void vdp_fill_rect(int x, int y, int w, int h, uint16_t tile);

#endif
