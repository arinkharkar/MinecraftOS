#ifndef __VIDEO__H__
#define __VIDEO__H__
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <minecraftOS.h>
#include <eightdata.h>
#include <error_handler.h>
#include <multiboot.h>
#include <asciibmp.h>
#include <stdlib.h>


#define CHAR_HEIGHT 13
#define CHAR_WIDTH 16

#define COLOR_BLACK   0
#define COLOR_RED     (rgb(255, 0, 0))
#define COLOR_BG      (rgb(87, 0, 61))

#define FRONT_BUFFER_DEFAULT_VAL ((uint32_t*)1)

typedef uint32_t color;

// height of the screen
extern uint32_t SCREEN_WIDTH;
// width of the screen
extern uint32_t SCREEN_HEIGHT;
// bits per pixel (color quality) of the screen
extern uint32_t SCREEN_BPP;

// pointer to the back buffer of video memory, do not draw to this, instead use plot_pixel
#define BACK_BUFFER_SIZE (1920*1080*4)
extern uint32_t back_buffer[BACK_BUFFER_SIZE];

// the pointer to video memory, do not draw to this, instead write to back buffer via plot_pixel and call swap()
extern uint32_t* front_buffer;

// initalizes the video driver, returns ERROR if there is an error
int init_video(multiboot_info_t* multiboot_info);

// plots pixel at (x,y) with color col, returns ERROR if there is an error
int plot_pixel(int x, int y, color col);
// plots pixel at (x,y) with color col, WARNING: this does no checks whatsoever, you must ensure x, y is a valid screen coord
void plot_pixel_f(int x, int y, color col);

// draws an individual character
int draw_char(char c);

// Verifies that video memory is all cleared
int verify_video_memory(int width, int height, int bpp, uint8_t* beginVideoMem);

// prints string to screen
int print_str(const char* str);
// prints int to screen
int print_int(const int num);
//prints a char to screen
int print_ch(const char c);
//prints a number in base 16 to screen
int print_hex(const int num);
//prints a bool to screen
int print_bool(bool b);
//prints a float to screen
int print_float(const float num);
//prints a float with a specified digit count to screen
int print_floatd(const float num, const int digits);
//prints a float at a location to screen
int print_float_inplace(const float num, const int x, const int y);
//prints an int at a location to screen
int print_int_inplace(const int num, const int x, const int y);
//prints a string at a location to screen
int print_str_inplace(const char* str, const int x, const int y);
//prints a bool at a location to screen
int print_bool_inplace(const bool b, const int x, const int y);
// prints a new line
int print_ln();

// fill the screen with the specified color
void fill_screen(color col);

// draws a vertical line
void draw_vertical_line(int yAxis, int len, color col);
// clears the screen to black
void clear_screen();

extern uint32_t vSz;

static inline void swap() {
    memcpy (front_buffer, back_buffer, vSz);
}

// draws an image given by an array from https://notisrac.github.io/FileToCArray/ WARNING: no bounds checks are done, the p_img_data must contain correct data
#ifdef __cplusplus
void draw_image(const uint32_t* p_img_data, const uint32_t width, const uint32_t height, const uint32_t startX = 0, const uint32_t startY = 0);
#else 
void draw_image(const uint32_t* p_img_data, const uint32_t width, const uint32_t height, const uint32_t startX, const uint32_t startY);
#endif

#ifdef __cplusplus
static inline constexpr color rgb(uint32_t r, uint32_t g, uint32_t b) {return ((r << 16) | (g << 8) | (b));};
#else
#define rgb(r, g, b) (((r) << 16) | ((g) << 8) | (b))
#endif
#endif