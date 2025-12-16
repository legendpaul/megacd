#include "vdp.h"
#include <stddef.h>

static void vdp_wait_ready(void) {
    while (VDP_CTRL_PORT & 0x8000) {}
}

static void vdp_set_reg(uint16_t reg, uint8_t value) {
    vdp_wait_ready();
    VDP_CTRL_PORT = 0x8000 | (reg << 8) | value;
}

static void vdp_set_addr(uint16_t addr, uint16_t reg) {
    VDP_CTRL_PORT = (addr & 0x3FFF) | (reg << 14);
    VDP_CTRL_PORT = ((addr >> 14) & 3);
}

void vdp_init(void) {
    vdp_set_reg(0x00, 0x04); // disable interrupts, H40
    vdp_set_reg(0x01, 0x74); // display enable, V interrupt
    vdp_set_reg(0x02, 0x30); // plane A at 0xC000
    vdp_set_reg(0x03, 0x00); // window
    vdp_set_reg(0x04, 0x07); // plane B at 0xE000
    vdp_set_reg(0x05, 0x7C); // sprites
    vdp_set_reg(0x07, 0x00); // bg color
    vdp_set_reg(0x0B, 0x00);
    vdp_set_reg(0x0C, 0x81); // H40
    vdp_set_reg(0x0F, 0x02); // auto inc 2
    vdp_set_reg(0x10, 0x01); // scroll size 64x32
    vdp_set_reg(0x11, 0x00);
    vdp_set_reg(0x12, 0x00);
    vdp_set_reg(0x13, 0x00);
    vdp_set_reg(0x14, 0x00);
}

static const uint32_t font_tiles[96*2] = {
#include "font.inc"
};

void vdp_upload_tiles(const uint32_t *data, uint16_t tile_index, uint16_t count) {
    uint32_t addr = tile_index * 32;
    vdp_set_addr(addr, 1);
    for (uint16_t i = 0; i < count * 8; ++i) {
        uint32_t v = data[i];
        VDP_DATA_PORT = v >> 16;
        VDP_DATA_PORT = v & 0xFFFF;
    }
}

void vdp_load_font(void) {
    vdp_upload_tiles(font_tiles, 32, 96);
}

void vdp_clear(void) {
    vdp_set_addr(PLANE_A_ADDR, 0);
    for (int i = 0; i < 64*32; ++i) {
        VDP_DATA_PORT = 0;
    }
}

void vdp_puts(int x, int y, const char *str) {
    uint32_t addr = PLANE_A_ADDR + 2 * (x + y * 64);
    vdp_set_addr(addr, 0);
    while (str && *str) {
        uint16_t tile = 32 + (*str - 32);
        VDP_DATA_PORT = tile;
        ++str;
    }
}

void vdp_set_palette(uint16_t index, uint16_t color) {
    uint16_t addr = index * 2;
    VDP_CTRL_PORT = 0xC000 | (addr & 0x3FFF);
    VDP_CTRL_PORT = (addr >> 14) & 3;
    VDP_DATA_PORT = color;
}

void vdp_fill_rect(int x, int y, int w, int h, uint16_t tile) {
    for (int row = 0; row < h; ++row) {
        uint32_t addr = PLANE_A_ADDR + 2 * ((x) + (y+row) * 64);
        vdp_set_addr(addr, 0);
        for (int col = 0; col < w; ++col) {
            VDP_DATA_PORT = tile;
        }
    }
}
