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
