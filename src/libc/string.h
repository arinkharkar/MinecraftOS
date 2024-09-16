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

static void* memcpy(void* dest, const void* src, size_t n) {
    // Cast pointers to char* for byte-wise operations
    char* d = (char*) dest;
    const char* s = (const char*) src;

    // Align address to 4-byte boundary
    while (((uintptr_t)d & (sizeof(uint32_t) - 1)) && n > 0) {
        *d++ = *s++;
        n--;
    }

    // Copy 4 bytes at a time
    uint32_t* dst = (uint32_t*)d;
    const uint32_t* src32 = (const uint32_t*)s;
    while (n >= sizeof(uint32_t)) {
        *dst++ = *src32++;
        n -= sizeof(uint32_t);
    }

    // Copy remaining bytes
    d = (char*)dst;
    s = (const char*)src32;
    while (n > 0) {
        *d++ = *s++;
        n--;
    }

    return dest;
}

#endif