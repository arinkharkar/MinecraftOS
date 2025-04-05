#ifndef __IO__H_
#define __IO__H_

#include <stdint.h>

/* outputs val on port */
static void outb(uint16_t port, uint8_t val) { 
    // vscode intelisense doesnt like inline assembly, so just add __VSCODE_ as one of the defines in c/c++ configs so that intelisense doesnt give incorrect errors here
    #ifndef __VSCODE_
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
    #endif
}



/* reads one byte from port port */
static unsigned char inb(uint16_t port){ 
    // vscode intelisense doesnt like inline assembly, so just add __VSCODE_ as one of the defines in c/c++ configs so that intelisense doesnt give incorrect errors here
    #ifndef __VSCODE_
    uint8_t returnVal;
    asm volatile ("inb %1, %0"
    : "=a"(returnVal)
    : "Nd"(port));
    return returnVal;
    #endif
}

/* reads 2 bytes from port port */
static uint16_t inw (uint16_t port) {
    // vscode intelisense doesnt like inline assembly, so just add __VSCODE_ as one of the defines in c/c++ configs so that intelisense doesnt give incorrect errors here
    #ifndef __VSCODE_
    uint16_t result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
    #endif
}


/* outputs val on port */
static void outw (unsigned short port, unsigned short data) {
    // vscode intelisense doesnt like inline assembly, so just add __VSCODE_ as one of the defines in c/c++ configs so that intelisense doesnt give incorrect errors here
    #ifndef __VSCODE_
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
    #endif
}

/* outputs val on port */
static void outl(uint32_t port, uint32_t value) {
    // vscode intelisense doesnt like inline assembly, so just add __VSCODE_ as one of the defines in c/c++ configs so that intelisense doesnt give incorrect errors here
    #ifndef __VSCODE_
    __asm__ __volatile__("outl %%eax,%%dx"::"d" (port), "a" (value));
    #endif
};

/* reads 4 bytes from port port */
static uint32_t inl(uint32_t port) {
    // vscode intelisense doesnt like inline assembly, so just add __VSCODE_ as one of the defines in c/c++ configs so that intelisense doesnt give incorrect errors here
    #ifndef __VSCODE_
    uint32_t result;
    __asm__ __volatile__("inl %%dx,%%eax":"=a" (result):"d"(port));
    return result;
    #endif
}




#endif