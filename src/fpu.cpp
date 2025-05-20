#include <minecraftOS.h>
#include "fpu.h"

// genuinely no idea what this does, also i dont remember where I got it but I did not write this
void init_fpu() {
    // vscode intelisense doesnt like inline assembly, so just add __VSCODE_ as one of the defines in c/c++ configs so that intelisense doesnt give incorrect errors here
    #ifndef __VSCODE_
    size_t t;

    asm("clts");
    asm("mov %%cr0, %0" : "=r"(t));
    t &= ~(1 << 2);
    t |= (1 << 1);
    asm("mov %0, %%cr0" :: "r"(t));
    asm("mov %%cr4, %0" : "=r"(t));
    t |= 3 << 9;
    asm("mov %0, %%cr4" :: "r"(t));
    asm("fninit");
    #endif
}