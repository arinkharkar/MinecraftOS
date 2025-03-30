#include "draw.h"


void draw_square(int x1, int y1, int x2, int y2, color col) {
    if (x1 < 0 || x1 > SCREEN_WIDTH || y1 < 0 || y1 > SCREEN_HEIGHT || x2 < 0 || x2 > SCREEN_WIDTH || y2 < 0 || y2 > SCREEN_HEIGHT || x1 > x2 || y2 < y1)
        return;
    for (int i = 0; i < x2 - x1; i++) {
        for (int j = 0; j < y2 - y1; j++) {
            plot_pixel_f(i + x1, j + y1, col);
        }
    }
}


void draw_line(int x1, int y1, int x2, int y2, color col) {
    // essentially, are the coords in bounds
   // if (x1 < 0 || x1 > SCREEN_WIDTH || y1 < 0 || y1 > SCREEN_HEIGHT || x2 < 0 || x2 > SCREEN_WIDTH || y2 < 0 || y2 > SCREEN_HEIGHT)
     //   return;
    float curX = x1;
    float curY = y1;
    float slope = fabs(((float)y2-y1)/((float)x2-x1));
    bool modifyY = true;
    if (slope < 1) {
        modifyY = false;
        slope = 1/slope;
    }
    
    if (curX == x2) {
        while (curY != y2) {
            plot_pixel((int)curX, (int)curY, col);
            curY++;
        }
        return;
    }

    if (curY == y2) {
        while (curX != x2) {
            plot_pixel((int)curX, (int)curY, col);
            curX++;
        }
        return;
    }

    // Make sure we dont loop unreasonably for some reason
    #define __MAX_SAFETY_COUNT__ 10000
    int safetyCounter = 0;
    while (curX != x2 && curY != y2) {
        plot_pixel((int)curX, (int)curY, col);
        if (modifyY) {
            curX++;
            float current_curY = curY;
            // go through and plot each pixel
            while (curY != current_curY + slope) {
                plot_pixel((int)curX, (int)curY, col);
                curY++;
            }
        } else {
            curY++;
            float current_curX = curX;
            // go through and plot each pixel
            int safetyCounter1 = 0;
            while (curX != current_curX + slope) {
                if (safetyCounter1 > __MAX_SAFETY_COUNT__)
                    break;
                plot_pixel((int)curX, (int)curY, col);
                curX++;
                safetyCounter1++;
            }
        }
        if (safetyCounter > __MAX_SAFETY_COUNT__)
            break;
        safetyCounter++;
    } 

}