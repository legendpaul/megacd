#ifndef MINLIB_H
#define MINLIB_H
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *s);
void *memset(void *dest, int value, size_t len);
void *memcpy(void *dest, const void *src, size_t len);
char *strncpy(char *dest, const char *src, size_t n);
int snprintf(char *buf, size_t size, const char *fmt, ...);

#endif
