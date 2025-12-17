#ifndef COMM_H
#define COMM_H

#include <stdint.h>

#define MAILBOX_BASE 0xFFF000

#define CMD_NONE 0
#define CMD_PING 1
#define CMD_LOAD_FILE 2
#define CMD_GET_CD_STATUS 3
#define CMD_PCM_START 4
#define CMD_PCM_STOP 5

#define STATUS_IDLE 0
#define STATUS_BUSY 1
#define STATUS_DONE 2
#define STATUS_ERROR 3

#define MAILBOX_PATH_MAX 64

typedef struct {
    volatile uint16_t command;
    volatile uint16_t status;
    volatile uint16_t error_code;
    volatile uint16_t reserved;
    volatile uint32_t arg0;
    volatile uint32_t arg1;
    volatile uint32_t arg2;
    volatile uint32_t arg3;
    volatile uint32_t buffer;
    volatile uint32_t length;
    volatile char path[MAILBOX_PATH_MAX];
} mailbox_t;

#define MAILBOX (*(mailbox_t*)MAILBOX_BASE)

static inline void mailbox_clear(void) {
    MAILBOX.command = CMD_NONE;
    MAILBOX.status = STATUS_IDLE;
    MAILBOX.error_code = 0;
    MAILBOX.arg0 = MAILBOX.arg1 = MAILBOX.arg2 = MAILBOX.arg3 = 0;
    MAILBOX.buffer = 0;
    MAILBOX.length = 0;
    for (int i = 0; i < MAILBOX_PATH_MAX; ++i) MAILBOX.path[i] = 0;
}

#endif
