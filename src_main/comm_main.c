#include "comm_main.h"
#include <string.h>

static const char *last_status = "IDLE";

void comm_init(void) {
    mailbox_clear();
}

static int wait_done(int timeout_frames) {
    while (timeout_frames-- > 0) {
        if (MAILBOX.status == STATUS_DONE || MAILBOX.status == STATUS_ERROR) return 1;
    }
    return 0;
}

cd_result_t comm_ping(void) {
    mailbox_clear();
    MAILBOX.command = CMD_PING;
    MAILBOX.status = STATUS_BUSY;
    if (!wait_done(600)) {
        last_status = "TIMEOUT";
        return CD_TIMEOUT;
    }
    last_status = MAILBOX.status == STATUS_DONE ? "OK" : "REMOTE ERR";
    return MAILBOX.status == STATUS_DONE ? CD_OK : CD_REMOTE_ERROR;
}

cd_result_t comm_load_file(const char *path, void *dest, uint32_t length, uint32_t *out_read) {
    mailbox_clear();
    strncpy((char*)MAILBOX.path, path, MAILBOX_PATH_MAX-1);
    MAILBOX.buffer = (uint32_t)dest;
    MAILBOX.length = length;
    MAILBOX.command = CMD_LOAD_FILE;
    MAILBOX.status = STATUS_BUSY;
    if (!wait_done(1200)) {
        last_status = "TIMEOUT";
        return CD_TIMEOUT;
    }
    if (MAILBOX.status == STATUS_DONE) {
        if (out_read) *out_read = MAILBOX.arg0;
        last_status = "OK";
        return CD_OK;
    }
    last_status = "REMOTE ERR";
    return CD_REMOTE_ERROR;
}

cd_result_t comm_pcm_start(void) {
    mailbox_clear();
    MAILBOX.command = CMD_PCM_START;
    MAILBOX.status = STATUS_BUSY;
    if (!wait_done(600)) return CD_TIMEOUT;
    return MAILBOX.status == STATUS_DONE ? CD_OK : CD_REMOTE_ERROR;
}

cd_result_t comm_pcm_stop(void) {
    mailbox_clear();
    MAILBOX.command = CMD_PCM_STOP;
    MAILBOX.status = STATUS_BUSY;
    if (!wait_done(600)) return CD_TIMEOUT;
    return MAILBOX.status == STATUS_DONE ? CD_OK : CD_REMOTE_ERROR;
}

const char *comm_status_text(void) {
    return last_status;
}
