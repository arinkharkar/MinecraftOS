#ifndef __PS2__MOUSE_H_
#define __PS2__MOUSE_H_

#include "io.h"

#define CURSOR_COL rgb(255, 255, 255)

void mouse_recv();

void draw_cursor();

int init_ps2_mouse();


typedef struct {
    // status_byte is a bit-flag, https://wiki.osdev.org/PS/2_Mouse for the values of each bit (also found in the defines under this struct)
    byte status_byte;
    byte x_movement;
    byte y_movement;
} mouse_data_t;

#define PS2_BUTTON_LEFT_FLAG    1<<0
#define PS2_BUTTON_RIGHT_FLAG   1<<1
#define PS2_BUTTON_MIDDLE_FLAG  1<<2
#define PS2_ALWAYS_ONE_FLAG     1<<3
#define PS2_X_SIGN_BIT          1<<4
#define PS2_Y_SIGN_BIT          1<<5
#define PS2_X_OVERFLOW_BIT      1<<6
#define PS2_Y_OVERFLOW_BIT      1<<7

#endif