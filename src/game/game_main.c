#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <video.h>
#include <ps2keyboard.h>
#include "draw.h"
#include "game_main.h"
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
    int speedDiv = 10;
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
   /* for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 0; z < 4; z++) {
                draw_cube(x*100, y*100, z*100, 100);
            }
        }
    }
    //draw_cube(0, 0, 0, 100);
    //draw_cube(100, 0, 0, 100);
   /* draw_line(0 + cameraPos.x, 0 + cameraPos.y, 20 + cameraPos.z, 1000, 0);
    draw_line(0 + cameraPos.x + 1000, 0 + cameraPos.y - 1000, 20 + cameraPos.z, 1000, 1);
    draw_line(0 + cameraPos.x, 0 + cameraPos.y - 1000, 20 + cameraPos.z, 1000, 1);
    draw_line(0 + cameraPos.x, 0 + cameraPos.y - 1000, 20 + cameraPos.z, 1000, 0);*/
    
    last_ticks = ticks;
}

float fov = 50;

void draw_line(float x1, float y1, float z1, float len , int dir) {  
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
        draw_line(x1 + cameraPos.x, y1 + cameraPos.y, z1 + cameraPos.z, sideLen, 2);

        draw_line(x1 + cameraPos.x, y1 + sideLen + cameraPos.y, z1 + cameraPos.z, sideLen, 2);

        draw_line(x1 + cameraPos.x, y1 + cameraPos.y, z1 + cameraPos.z, sideLen, 1);

        draw_line(x1 + cameraPos.x, y1 + cameraPos.y, z1 + sideLen + cameraPos.z, sideLen, 1);
    } else if (y2 -  y1 == 0) {
        sideLen = x2 - x1;
        draw_line(x1 + cameraPos.x, y1 + cameraPos.y, z1 + cameraPos.z, sideLen, 0);

        draw_line(x1 + cameraPos.x, y1 + cameraPos.y, z1 + cameraPos.z + sideLen, sideLen, 0);

        draw_line(x1 + cameraPos.x, y1 + cameraPos.y, z1 + cameraPos.z, sideLen, 2);

        draw_line(x1 + sideLen + cameraPos.x, y1 + cameraPos.y, z1 + cameraPos.z, sideLen, 2);
    } else if (z2 - z1 == 0) {
        sideLen = y2 - y1;
        draw_line(x1 + cameraPos.x, y1 + cameraPos.y, z1 + cameraPos.z, sideLen, 0);

        draw_line(x1 + cameraPos.x, y1 + cameraPos.y + sideLen, z1 + cameraPos.z, sideLen, 0);

        draw_line(x1 + cameraPos.x, y1 + cameraPos.y, z1 + cameraPos.z, sideLen, 1);

        draw_line(x1 + cameraPos.x + sideLen, y1 + cameraPos.y, z1 + cameraPos.z, sideLen, 1);
        
    }
}