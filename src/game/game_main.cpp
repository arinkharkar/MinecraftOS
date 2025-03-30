#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <video.h>
#include <math.h>
#include <ps2keyboard.h>
#include "draw.h"
#include "game_main.h"
#include <matrix.h>
#include "pit.h"

int sPosx;
int sPosy;

#define WORLD_LEN 16

typedef uint32_t block;
#define GRASS 0x236533
#define COBBLE 0x808080

block world[WORLD_LEN][WORLD_LEN];

uint32_t last_ticks;

vector3 cameraPos = {0};

void game_init() {
    print_str("Initializing Minecraft...");

    for (int i = 0; i < WORLD_LEN; i++) {
        for (int j = 0; j < WORLD_LEN; j++) {
            if (j > 8)
                world[i][j] = COBBLE;
            else
                world[i][j] = GRASS;
        }
            
    }

    


    print_str("Ready, press Space to start");
    swap();
    while (!is_key_down(' ')) {}
    clear_screen();
    last_ticks = ticks;
}


void game_loop() {
    int deltaTime = ticks - last_ticks;
    
    // deltaTime is the time per frame, convert this to seconds and invert it for frames per second
    float fps = 1 / ((float)deltaTime / 1000);
    
    int speedDiv = 3;
    if (is_key_down('a'))
        cameraPos.x += deltaTime * speedDiv;
    if (is_key_down('d'))
        cameraPos.x -= deltaTime * speedDiv;
    if (is_key_down('w'))
        cameraPos.z -= deltaTime * speedDiv;
    if (is_key_down('s'))
        cameraPos.z += deltaTime * speedDiv;
    if (is_key_down('c'))
        cameraPos.y -= deltaTime * speedDiv;
    if (is_key_down(' '))
        cameraPos.y += deltaTime * speedDiv;

    draw_square(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xADD8E6);

    float fov = 68;
    float S = 1 / (tan(fov * M_PI / 360));
    float f = 300;
    float n = 0.01;

    matrix_m4s proj_m = {S, 0, 0, 0,
                         0, S, 0, 0,
                         0, 0, -f/(f-n), -1,
                         0, 0, - (f*n)/(f-n), 1};
    
    vector4 worldPoint1 = {1, 0, 0, 1};
    vector4 worldPoint2 = {0, 0.1, 0, 1};

    vector4 screenPoint1 = {0};
    vector4 screenPoint2 = {0};



    multiply_m4sv(proj_m, worldPoint1, screenPoint1);
    multiply_m4sv(proj_m, worldPoint2, screenPoint2);
    
   

    screenPoint1[0] = (screenPoint1[0] / screenPoint1[3] + 1) / 2 * SCREEN_WIDTH;
    screenPoint2[0] = (screenPoint2[0] / screenPoint2[3] + 1) / 2 * SCREEN_WIDTH;

    screenPoint1[1] = (1 - screenPoint1[1] / screenPoint1[3]) / 2 * SCREEN_HEIGHT;
    screenPoint2[1] = (1 - screenPoint2[1] / screenPoint2[3]) / 2 * SCREEN_HEIGHT;

    print_float_inplace(screenPoint1[0], 0, 20);
    print_float_inplace(screenPoint1[1], 100, 20);
    print_float_inplace(screenPoint1[2], 200, 20);
    print_float_inplace(screenPoint1[3], 300, 20);

    print_float_inplace(screenPoint2[0], 0, 40);
    print_float_inplace(screenPoint2[1], 100, 40);
    print_float_inplace(screenPoint2[2], 200, 40);
    print_float_inplace(screenPoint2[3], 300, 40);
    //screenPoint1[0] = 0;
    print_float_inplace((screenPoint2[1] - screenPoint1[1]) / (screenPoint2[0] - screenPoint1[0]), 400, 40);
 //   screenPoint1[0] = 0;
    draw_line(screenPoint1[0], screenPoint1[1], screenPoint2[0], screenPoint2[1], rgb(129, 37, 175));

   
    

    /*
    float fov = 50;
    float x1 = 0 + cameraPos.x; float y1 = 0 + cameraPos.y; float z1 = 0 + cameraPos.z;
    float x2 = 100 + cameraPos.x; float y2 = 100 + cameraPos.y; float z2 = cameraPos.z;

    float projectedX1 = (x1) * fov / (fov + z1) + SCREEN_WIDTH / 2;
    float projectedY1 = y1 * fov / (fov + z1) + SCREEN_HEIGHT / 2;
    float projectedX2 = (x2) * fov / (fov + z2) + SCREEN_WIDTH / 2;
    float projectedY2 = y2 * fov / (fov + z2) + SCREEN_HEIGHT / 2;
    
    draw_line(projectedX1, projectedY1, projectedX2, projectedY2, rgb(124, 0, 0));
*/





   // draw_line_f(0 + cameraPos.x, 0 + cameraPos.y, cameraPos.z, 10, 1);

   // draw_cube(0, 0, 0, 20);
    print_float_inplace(fps, 0, 0);
    last_ticks = ticks;
  //  draw_line_f(0, 0, 0, 10, 0);
   /* for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 0; z < 4; z++) {
                draw_cube(x*100, y*100, z*100, 100);
            }
        }
    }
    //draw_cube(0, 0, 0, 100);
    //draw_cube(100, 0, 0, 100);
   /* draw_line_f(0 + cameraPos.x, 0 + cameraPos.y, 20 + cameraPos.z, 1000, 0);
    draw_line_f_f(0 + cameraPos.x + 1000, 0 + cameraPos.y - 1000, 20 + cameraPos.z, 1000, 1);
    draw_line_f_f(0 + cameraPos.x, 0 + cameraPos.y - 1000, 20 + cameraPos.z, 1000, 1);
    draw_line_f_f(0 + cameraPos.x, 0 + cameraPos.y - 1000, 20 + cameraPos.z, 1000, 0);*/
    
    
}

float fov = 50;

void draw_line_f(float x1, float y1, float z1, float len , int dir) {  
    if (!dir) { 
        for (int i = 0; i < len; i++) {
            float projectedX = (x1 + i) * fov / (fov + z1) + SCREEN_WIDTH / 2;
            float projectedY = y1 * fov / (fov + z1) + SCREEN_HEIGHT / 2;
            plot_pixel(projectedX, projectedY, 0x0);
        }
    } else if (dir == 1) {
        for (int i = 0; i < len; i++) {
            float projectedX = (x1) * fov / (fov + z1) + SCREEN_WIDTH / 2;
            float projectedY = (y1 + i) * fov / (fov + z1) + SCREEN_HEIGHT / 2;
            plot_pixel(projectedX, projectedY, 0x0);
        }
    } else if (dir == 2) {
        for (int i = 0; i < len; i++) {
            float projectedX = (x1) * fov / (fov + z1 + i) + SCREEN_WIDTH / 2;
            float projectedY = (y1) * fov / (fov + z1 + i) + SCREEN_HEIGHT / 2;
            plot_pixel(projectedX, projectedY, 0x0);
        }
    }
}

void draw_cube(float x1, float y1, float z1, float len) {
    draw_hollow_square(x1, y1, z1, x1, y1+len, z1+len);
    draw_hollow_square(x1+len, y1, z1, x1+len, y1+len, z1+len);
    draw_hollow_square(x1, y1, z1, x1+len, y1, z1+len);
    draw_hollow_square(x1, y1+len, z1, x1+len, y1+len, z1+len);
    draw_hollow_square(x1, y1, z1, x1+len, y1+len, z1);
    draw_hollow_square(x1, y1, z1, x1+len, y1+len, z1+len);
}

void draw_hollow_square(float x1, float y1, float z1, float x2, float y2, float z2) {
    float sideLen = 0;
    if (x2 - x1 == 0) {
        sideLen = y2 - y1;
        draw_line_f(x1, y1, z1, sideLen, 2);

        draw_line_f(x1, y1 + sideLen, z1, sideLen, 2);

        draw_line_f(x1, y1, z1, sideLen, 1);

        draw_line_f(x1, y1, z1 + sideLen, sideLen, 1);
    } else if (y2 -  y1 == 0) {
        sideLen = x2 - x1;
        draw_line_f(x1, y1, z1, sideLen, 0);

        draw_line_f(x1, y1, z1 + sideLen, sideLen, 0);

        draw_line_f(x1, y1, z1, sideLen, 2);

        draw_line_f(x1 + sideLen, y1, z1, sideLen, 2);
    } else if (z2 - z1 == 0) {
        sideLen = y2 - y1;
        draw_line_f(x1, y1, z1, sideLen, 0);

        draw_line_f(x1, y1 + sideLen, z1, sideLen, 0);

        draw_line_f(x1, y1, z1, sideLen, 1);

        draw_line_f(x1 + sideLen, y1, z1, sideLen, 1);
        
    }
}