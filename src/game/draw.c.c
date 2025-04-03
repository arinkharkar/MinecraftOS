#include <stdlib.h>
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


void draw_line(int x0, int y0, int x1, int y1, color col) {
// Uses Bresenham's line algorithm from https://gist.github.com/bert/1085538 to draw a line between 2 points
    int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2; /* error value e_xy */
    
    for (;;){  /* loop */
        plot_pixel (x0,y0, col);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }

}