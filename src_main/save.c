#include "save.h"
#include "../src_common/minlib.h"
#include <stddef.h>

static save_data_t g_save;

static uint16_t checksum_bytes(const uint8_t *data, size_t len) {
    uint32_t sum = 0;
    for (size_t i = 0; i < len; ++i) sum += data[i];
    return (uint16_t)(sum & 0xFFFF);
}

void save_init(void) {
    memset(&g_save, 0, sizeof(g_save));
    g_save.magic = 0x53415645; // 'SAVE'
    g_save.version = 1;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 16; ++j) g_save.slots[i][j] = (uint8_t)(i*16+j);
    }
    save_compute_checksum(&g_save);
}

void save_compute_checksum(save_data_t *data) {
    data->checksum = 0;
    data->checksum = checksum_bytes((const uint8_t*)data, sizeof(*data));
}

int save_verify(const save_data_t *data) {
    uint16_t cs = checksum_bytes((const uint8_t*)data, sizeof(*data));
    return cs == data->checksum;
}

void save_write_slot(int slot, const uint8_t *payload, uint16_t len) {
    if (slot < 0 || slot >=3) return;
    if (len > sizeof(g_save.slots[slot])) len = sizeof(g_save.slots[slot]);
    memcpy(g_save.slots[slot], payload, len);
    save_compute_checksum(&g_save);
}

const save_data_t *save_get(void) { return &g_save; }
