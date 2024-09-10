#ifndef ___STRING__H__
#define ___STRING__H__

#include <stdint.h>
#include <stddef.h>

static void *memset(void *ptr, int value, size_t num) {
    unsigned char *p = (unsigned char *)ptr;
    unsigned char val = (unsigned char)value;

    for (size_t i = 0; i < num; i++) {
        p[i] = val;
    }

    return ptr;
}

#endif