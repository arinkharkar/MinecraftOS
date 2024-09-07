#include "video.h" 



int init_video(multiboot_info_t* multiboot_info) {
    // ensure the video memory is correct by making sure it is currently empty
    uint8_t* checkPtr = (uint8_t*)multiboot_info->framebuffer_addr;
    int w, h, bpp;
    w = multiboot_info->framebuffer_width;
    h = multiboot_info->framebuffer_height;
    bpp = multiboot_info->framebuffer_bpp;
    
    // go through each byte of video memory and ensure it is zero, otherwise return an error
    for (int i = 0; i < w * h * bpp / BITS_PER_BYTE; i++) {
        if (*checkPtr != 0) {
            set_last_error("Error Initializing Video Memory");
            return ERROR;
        }
    }
    
    video_memory = (uint32_t)multiboot_info->framebuffer_addr;
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;
    SCREEN_BPP = bpp;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            plot_pixel(i, j, eightData[j * 16 + i]);
        }
    }
    return SUCCESS;
}


int plot_pixel(int x, int y, uint32_t col) {
    if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
        set_last_error("Error plotting pixel: x or y out of range");
        return ERROR;
    }
    video_memory[y * SCREEN_WIDTH + x] = col;
    return SUCCESS;
}
