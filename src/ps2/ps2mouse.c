#include "ps2mouse.h"
#include <video.h>
#include "cursor.h"

int mCoordx = 500;
int mCoordy = 500;
int lmCoordx = 500;
int lmCoordy = 500;

void mouse_recv() {
    uint8_t b1 = inb(0x60);
    uint8_t xMov = inb(0x60);
    uint8_t yMov = inb(0x60);
    if (b1 & 0b11000000)
        return;
    uint8_t state = b1;
	uint8_t d = xMov;
	int rel_x = d - ((state << 4) & 0x100);
	d = yMov;
	int rel_y = d - ((state << 3) & 0x100);
    if (mCoordx + rel_x >= SCREEN_WIDTH || mCoordx + rel_x <= 0) {}
    else
        mCoordx += rel_x;
    if (mCoordy - rel_y >= SCREEN_HEIGHT || mCoordy - rel_y <= 0) {}
    else
        mCoordy -= rel_y;

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            plot_pixel(i+lmCoordx, j+lmCoordy, rgb(0, 140, 227));
        }
    }  
    lmCoordx = mCoordx;
    lmCoordy = mCoordy;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (cursor[j+i*20])
                plot_pixel(i+mCoordx, j+mCoordy, 0x0);
        }
    }  
}