#include "minlib.h"
#include <stdarg.h>

size_t strlen(const char *s) {
    size_t n = 0;
    while (s && *s) { ++n; ++s; }
    return n;
}

void *memset(void *dest, int value, size_t len) {
    uint8_t *d = (uint8_t*)dest;
    for (size_t i = 0; i < len; ++i) d[i] = (uint8_t)value;
    return dest;
}

void *memcpy(void *dest, const void *src, size_t len) {
    uint8_t *d = (uint8_t*)dest;
    const uint8_t *s = (const uint8_t*)src;
    for (size_t i = 0; i < len; ++i) d[i] = s[i];
    return dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
    size_t i = 0;
    for (; i < n && src[i]; ++i) dest[i] = src[i];
    for (; i < n; ++i) dest[i] = '\0';
    return dest;
}

static void append_char(char **buf, size_t *remaining, char c, int *count) {
    if (*remaining > 1) {
        **buf = c;
        (*buf)++;
        (*remaining)--;
    }
    (*count)++;
}

static void append_str(char **buf, size_t *remaining, const char *s, int *count) {
    while (s && *s) {
        append_char(buf, remaining, *s++, count);
    }
}

static void append_uint(char **buf, size_t *remaining, unsigned int value, unsigned int base, int pad, char padch, int uppercase, int *count) {
    char tmp[16];
    int pos = 0;
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    do {
        tmp[pos++] = digits[value % base];
        value /= base;
    } while (value && pos < (int)sizeof(tmp));
    while (pos < pad) tmp[pos++] = padch;
    for (int i = pos - 1; i >= 0; --i) append_char(buf, remaining, tmp[i], count);
}

int snprintf(char *buf, size_t size, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char *out = buf;
    size_t remain = size ? size : 1;
    int written = 0;
    while (fmt && *fmt) {
        if (*fmt != '%') {
            append_char(&out, &remain, *fmt++, &written);
            continue;
        }
        ++fmt;
        int pad = 0;
        char padch = ' ';
        if (*fmt == '0') { padch = '0'; ++fmt; }
        while (*fmt >= '0' && *fmt <= '9') { pad = pad*10 + (*fmt - '0'); ++fmt; }
        switch(*fmt) {
            case 's': {
                const char *s = va_arg(ap, const char*);
                append_str(&out, &remain, s, &written);
                break;
            }
            case 'd': {
                int v = va_arg(ap, int);
                if (v < 0) { append_char(&out, &remain, '-', &written); v = -v; }
                append_uint(&out, &remain, (unsigned int)v, 10, pad, padch, 0, &written);
                break;
            }
            case 'u': {
                unsigned int v = va_arg(ap, unsigned int);
                append_uint(&out, &remain, v, 10, pad, padch, 0, &written);
                break;
            }
            case 'X': {
                unsigned int v = va_arg(ap, unsigned int);
                append_uint(&out, &remain, v, 16, pad, padch, 1, &written);
                break;
            }
            case 'c': {
                int v = va_arg(ap, int);
                append_char(&out, &remain, (char)v, &written);
                break;
            }
            case '%':
                append_char(&out, &remain, '%', &written);
                break;
            default:
                append_char(&out, &remain, '?', &written);
                break;
        }
        ++fmt;
    }
    if (size > 0) {
        if (remain > 0) *out = '\0';
        else buf[size-1] = '\0';
    }
    va_end(ap);
    return written;
}
