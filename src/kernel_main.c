#include <stdint.h>
#include <video.h>
#include <gdt_manager.h>
#include <idt.h>
#include <game_main.h>
#include <math.h>
#include "pit.h"
#include "multiboot.h"
#include "ps2keyboard.h"
#include <string.h>



uint32_t vSz;
void init_fpu();
void game_loop();
/*
 * Called by start.S, gets the multiboot header from GRUB plus the magic number to ensure everything went smoothly
*/
void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    if (init_video(mbd) == ERROR) {
        // Cant print a message as the video hasnt been inited
        while(1) {}
    } else {
        print_str("Initalized Video!\n");
    }
    vSz = SCREEN_BPP / BITS_PER_BYTE * SCREEN_WIDTH * SCREEN_HEIGHT;
    if(init_gdt() == ERROR) {
        print_str("Error Enabling GDT\n");
        print_str(get_last_error()); 
    } else {
        print_str("Enabled GDT!\n");
    }
    if (init_ps2keyboard() == ERROR) {
        print_str("Error with keyboard\n");
    } else {
        print_str("Enabled Keyboard\n");
    }
    if (init_idt() == ERROR) {
        print_str("Error Enabling IDT\n");
        print_str(get_last_error());
    } else {
        print_str("Enabled Interupts!\n");
    }
    init_fpu();
    print_str("\nResolution: ");
    print_int(SCREEN_WIDTH);
    print_str("x");
    print_int(SCREEN_HEIGHT);
   // init_pit(1000);
    memcpy(front_buffer, back_buffer, vSz);
    game_init();

    memcpy(front_buffer, back_buffer, vSz);

    while (1) {
        memset(back_buffer, 0x0, vSz);
        game_loop();

        memcpy(front_buffer, back_buffer, vSz);
    }
}



void init_fpu() {
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
}
