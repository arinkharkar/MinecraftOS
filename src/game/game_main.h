#ifndef _GAME_MAIN_H
#define _GAME_MAIN_H

void game_init();

void game_loop();

enum line_direction { xDir, yDir, zDir };

void draw_line_f(float x1, float y1, float z1, float len , int dir) ;

void draw_hollow_square(float x1, float y1, float z1, float x2, float y2, float z2);

void draw_cube(float x1, float y1, float z1, float len);

#endif 