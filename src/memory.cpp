#include <stddef.h>
#include <stdint.h>
#include "video.h"
#include <string.h>

#ifndef HEAP_SIZE
#define HEAP_SIZE (1048576*5)
#endif

char heap[HEAP_SIZE] = {0};
char* usedMemory = heap;

void* malloc(size_t c) {
    void* rVal = usedMemory;
    usedMemory += c;
    if (usedMemory >= &heap[HEAP_SIZE]) {
        print_str("ERROR: OUT OF HEAP SPACE");
        asm("cli;hlt");
    }
    return rVal;
}

void* calloc(size_t number, size_t size) {
    void* rVal = usedMemory;
    memset(usedMemory, 0, number * size);
    usedMemory += number * size;

    if (usedMemory >= &heap[HEAP_SIZE]) {
        print_str("ERROR: OUT OF HEAP SPACE");
        asm("cli;hlt");
    }
    return rVal;
}

void* operator new(size_t, void* p)      { return p; }
void* operator new[](size_t, void* p)    { return p; }
void* operator new(size_t size)          { return malloc(size); }

// TODO: Hell nah who do you think i am 😭🙏 we dont need delete
void operator delete(void* p)            {  }
void operator delete(void* p, size_t)    {  }