#ifndef _GAME_MAIN_H
#define _GAME_MAIN_H

void game_init();

void game_loop();

typedef struct {
    float x;
    float y;
    float z;
} vector3;

void draw_line(float x1, float y1, float z1, float len , int dir) ;

#endif 