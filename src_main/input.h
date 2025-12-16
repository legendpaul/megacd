#ifndef INPUT_H
#define INPUT_H
#include <stdint.h>

typedef struct {
    uint16_t raw;
    uint8_t up, down, left, right, a, b, c, start;
} pad_state_t;

void input_init(void);
void input_read(pad_state_t *out);

#endif
