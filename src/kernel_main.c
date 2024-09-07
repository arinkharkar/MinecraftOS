#include <stdint.h>
#include "multiboot.h"
#include "video.h"


/*
 * Called by start.S, gets the multiboot header from GRUB plus the magic number to ensure everything went smoothly
*/
void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    init_video(mbd);
    
    while (1) {}
}

/*void video_init(multiboot_info_t* multiboot_info) {
    uint32_t* a = multiboot_info->framebuffer_addr;
    int x = 0;
    for (x = 0; x < 720*1280;x++) 
        a[x] = 0xAA3333;
    a[x-1] = 0x0;
    a[x-2] = 0x0;
    a[x-3] = 0x0;
    a[x-4] = 0x0;
}*/