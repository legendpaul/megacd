#ifndef SAVE_H
#define SAVE_H
#include <stdint.h>

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t checksum;
    uint8_t options;
    uint8_t reserved[3];
    uint8_t slots[3][16];
} save_data_t;

void save_init(void);
void save_compute_checksum(save_data_t *data);
int save_verify(const save_data_t *data);
void save_write_slot(int slot, const uint8_t *payload, uint16_t len);
const save_data_t *save_get(void);

#endif
