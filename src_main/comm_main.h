#ifndef COMM_MAIN_H
#define COMM_MAIN_H
#include <stdint.h>
#include "../src_common/comm.h"

typedef enum {
    CD_OK = 0,
    CD_TIMEOUT = 1,
    CD_REMOTE_ERROR = 2
} cd_result_t;

void comm_init(void);
cd_result_t comm_ping(void);
cd_result_t comm_load_file(const char *path, void *dest, uint32_t length, uint32_t *out_read);
cd_result_t comm_pcm_start(void);
cd_result_t comm_pcm_stop(void);
const char *comm_status_text(void);

#endif
