#include <stdint.h>
#include <video.h>
#include <gdt_manager.h>
#include <idt.h>
#include "pit.h"
#include "multiboot.h"
#include "ps2keyboard.h"



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

    if(init_gdt() == ERROR) {
        print_str("Error Enabling GDT\n");
        print_str(get_last_error()); 
    } else {
        print_str("Enabled GDT!\n");
    }
     if (init_ps2keyboard() == ERROR) {
        print_str("Error Initializing Keyboard!\n");
    } else {
        print_str("Initialized Keyboard!");
    }
    if (init_idt() == ERROR) {
        print_str("Error Enabling IDT\n");
        print_str(get_last_error());
    } else {
        print_str("Enabled Interupts!\n");
    }
   
    init_pit(1000);
    
    
    while (1) {}
}