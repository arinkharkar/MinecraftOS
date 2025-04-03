#include <video.h>
#include <draw.h>
#include "ps2_helper.h"
#include "ps2mouse.h"
#include "cursor.h"

int mCoordx = 500;
int mCoordy = 500;
int lmCoordx = 500;
int lmCoordy = 500;

int init_ps2_mouse() {
    // Set the deafult settings of the mouse
    ps2_controller_waittowrite();
    write_to_mouse(PS2_DEVICE_SET_DEFAULTS);
    ps2_controller_waittoread();  
    byte c = recv_from_mouse();
    if (c != PS2_ACK)
        return ERROR;

    // Enable keyboard scanning
    ps2_controller_waittowrite();
    write_to_mouse(PS2_DEVICE_ENABLE_SCANNING);
    ps2_controller_waittoread();  
    c = recv_from_mouse();
    if (c != PS2_ACK)
        return ERROR;

    const int sample_rate = 200;
    // Enable keyboard scanning
    ps2_controller_waittowrite();
    write_to_mouse(PS2_MOUSE_SET_SAMPLE_RATE);
    ps2_controller_waittoread();  
    c = recv_from_mouse();
    if (c != PS2_ACK)
        return ERROR;
    ps2_controller_waittowrite();
    write_to_mouse(sample_rate);
    ps2_controller_waittoread();  
    c = recv_from_mouse();
    if (c != PS2_ACK)
        return ERROR;

    return SUCCESS;   
}

extern int sec;
void mouse_recv() {
    uint8_t b1 = recv_from_mouse();
    uint8_t xMov = recv_from_mouse();
    uint8_t yMov = recv_from_mouse();
    if (b1 & 0b11000000) {
        print_int_inplace(sec, 230, 600);
       // return;
    }


    uint8_t state = b1;
	uint8_t d = xMov;
	int rel_x = d - ((state << 4) & 0x100);
	d = yMov;
	int rel_y = d - ((state << 3) & 0x100);

    if (!(b1 & 0b00001000)) {
        print_str_inplace("ERROR BYTE", 230, 600);
        print_int_inplace(rel_x, 280, 700);
        print_int_inplace(rel_y, 300, 750);
        return;
    }

    if (mCoordx + rel_x >= SCREEN_WIDTH || mCoordx + rel_x <= 0) {}
    else
        mCoordx += rel_x;
    if (mCoordy - rel_y >= SCREEN_HEIGHT || mCoordy - rel_y <= 0) {}
    else
        mCoordy -= rel_y;
 //   print_str("mouse_recv()");
   // swap();
}


void draw_cursor() {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (cursor[j+i*20])
                plot_pixel(i+mCoordx, j+mCoordy, rgb(0, 0, 0));
        }
    }  
}