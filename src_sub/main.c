#include <stdint.h>
#include "../src_common/minlib.h"
#include "../src_common/comm.h"

#define WORD_RAM ((uint8_t*)0x200000)

static void mailbox_reply_done(uint16_t error, uint32_t extra) {
    MAILBOX.error_code = error;
    MAILBOX.arg0 = extra;
    MAILBOX.status = error ? STATUS_ERROR : STATUS_DONE;
    MAILBOX.command = CMD_NONE;
}

static void emulate_iso_read(const char *path, uint8_t *dest, uint32_t len) {
    /* Minimal fallback reader: fills buffer with recognizable pattern. */
    const char *msg = "ISO STUB DATA FROM SUB CPU";
    uint32_t mlen = strlen(msg);
    for (uint32_t i = 0; i < len; ++i) {
        dest[i] = msg[i % mlen];
    }
}

static void handle_command(void) {
    switch(MAILBOX.command) {
        case CMD_PING:
            mailbox_reply_done(0, 0);
            break;
        case CMD_LOAD_FILE: {
            uint8_t *dest = (uint8_t*)MAILBOX.buffer;
            uint32_t len = MAILBOX.length;
            emulate_iso_read((const char*)MAILBOX.path, dest, len);
            mailbox_reply_done(0, len);
            break;
        }
        case CMD_PCM_START:
        case CMD_PCM_STOP:
            mailbox_reply_done(0, 0);
            break;
        case CMD_GET_CD_STATUS:
            mailbox_reply_done(0, 0);
            break;
        default:
            break;
    }
}

int main(void) {
    mailbox_clear();
    while (1) {
        if (MAILBOX.command != CMD_NONE && MAILBOX.status == STATUS_BUSY) {
            handle_command();
        }
    }
    return 0;
}
