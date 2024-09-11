#ifndef __VIDEO__H__
#define __VIDEO__H__
#include <stdint.h>
#include <doomOS_types.h>
#include <eightdata.h>
#include <error_handler.h>
#include <multiboot.h>
#include <asciibmp.h>
#include <stdlib.h>

#define CHAR_HEIGHT 13
#define CHAR_WIDTH 16

typedef uint32_t color;

// height of the screen
extern uint32_t SCREEN_WIDTH;
// width of the screen
extern uint32_t SCREEN_HEIGHT;
// bits per pixel (color quality) of the screen
extern uint32_t SCREEN_BPP;

// pointer to video memory, do not draw to this, instead use plot_pixel
extern uint32_t back_buffer[1920*1080];

//extern uint32_t* back_buffer;

extern uint32_t* front_buffer;

// initalizes the video driver, returns ERROR if there is an error
int init_video(multiboot_info_t* multiboot_info);

// plots pixel at (x,y) with color col, returns ERROR if there is an error
int plot_pixel(int x, int y, color col);
// plots pixel at (x,y) with color col, WARNING: this does no checks whatsoever, you must ensure x, y is a valid screen coord
void plot_pixel_f(int x, int y, color col);

int draw_char(char c);
// Verifies that video memory is all cleared
int verify_video_memory(int width, int height, int bpp, uint8_t* beginVideoMem);

// prints string to screen
int print_str(const char* str);

int print_int(int num);

int print_hex(int num);

void draw_moving_img();

void draw_vertical_line(int yAxis, int len, color col);
// clears the screen to black
void clear_screen();

static color rgb(int r, int g, int b) {return ((r << 16) | (g << 8) | (b));};
#endif