#ifndef __VIDEO__H__
#define __VIDEO__H__
#include <stdint.h>
#include <doomOS_types.h>
#include <eightdata.h>
#include <error_handler.h>
#include <multiboot.h>

static uint32_t SCREEN_WIDTH;
static uint32_t SCREEN_HEIGHT;
static uint32_t SCREEN_BPP;

static uint32_t* video_memory;


int init_video(multiboot_info_t* multiboot_info);

int plot_pixel(int x, int y, uint32_t col);



#endif