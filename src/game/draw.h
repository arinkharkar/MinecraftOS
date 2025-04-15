#ifndef _DRAW_H_
#define _DRAW_H_

#include <video.h>
#include <string.h>
#include <stdbool.h>

extern uint32_t vSz;

void draw_square(int x1, int y1, int x2, int y2, color col);

void draw_line(int x1, int y1, int x2, int y2, color col);

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color col);

#endif