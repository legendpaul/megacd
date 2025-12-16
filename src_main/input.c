#include "input.h"

#define IO_DATA    (*(volatile uint8_t*)0xA10003)
#define IO_CTRL    (*(volatile uint8_t*)0xA10009)

void input_init(void) {
    IO_CTRL = 0x40; // TH control
}

void input_read(pad_state_t *out) {
    uint16_t value = 0;
    IO_DATA = 0x40; // TH high
    asm volatile ("nop\nnop\nnop");
    uint8_t v1 = IO_DATA;
    IO_DATA = 0x00; // TH low
    asm volatile ("nop\nnop\nnop");
    uint8_t v2 = IO_DATA;
    value = (v1 << 8) | v2;
    out->raw = value;
    out->up = !(v1 & 0x01);
    out->down = !(v1 & 0x02);
    out->left = !(v1 & 0x04);
    out->right = !(v1 & 0x08);
    out->b = !(v1 & 0x10);
    out->c = !(v1 & 0x20);
    out->a = !(v2 & 0x10);
    out->start = !(v2 & 0x20);
}
