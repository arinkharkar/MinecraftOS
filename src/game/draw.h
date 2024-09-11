#ifndef _DRAW_H_
#define _DRAW_H_

#include <video.h>
#include <string.h>

extern uint32_t vSz;

void draw_square(int x1, int y1, int x2, int y2, color col);

static inline void swap() {
    memcpy (front_buffer, back_buffer, vSz);
}


#endif