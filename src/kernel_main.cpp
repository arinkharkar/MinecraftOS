#include <stdint.h>
#include <string.h>
#include <math.h>
#include <matrix.h>
#include <video.h>
#include <math.h>
#include <gdt_manager.h>
#include <idt.h>
#include <game_main.h>
#include <minecraftOS.h>
#include "pit.h"
#include "multiboot.h"
#include "ps2keyboard.h"
#include "ps2_helper.h"
#include "ps2mouse.h"
#include "fpu.h"


void game_loop();

/*
 * Called by start.S, gets the multiboot header from GRUB plus the magic number to ensure everything went smoothly
*/
EXTERN_C NORETURN void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    if (init_video(mbd) == ERROR) {
        // Cant print a message as the video hasnt been inited
        PANIC("Couldn't Initialize Video");
    } else {
        print_str("Initalized Video!\n");
    }
    
    if(init_gdt() == ERROR) {
        print_str("Error Enabling GDT\n");
        print_str(get_last_error()); 
    } else {
        print_str("Enabled GDT!\n");
    }
    if (init_ps2controller() == ERROR) {
        print_str("Error with PS/2 Controller\n");
    }
    if (init_ps2keyboard() == ERROR) {
        print_str("Error with keyboard\n");
    } else {
        print_str("Enabled Keyboard\n");
    }
    if (init_ps2_mouse() == ERROR) {
        print_str("Error with mouse\n");
    } else {
        print_str("Enabled Mouse\n");
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
    print_str("\n");

    srand(ticks_passed);

    init_pit(1000);
    swap();

    game_init();

    while (1) {
        game_loop();
        draw_cursor();
        check_for_kybd_inactivity();
        swap();
    }
}
