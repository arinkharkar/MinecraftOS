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
    if (is_key_down(KEY_LSHIFT))
        cameraPos.y -= deltaTime * speedDiv;
    if (is_key_down(' '))
        cameraPos.y += deltaTime * speedDiv;

    draw_square(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xADD8E6);
    draw_line(0 + cameraPos.x, 0 + cameraPos.y, 20 + cameraPos.z, 1000, 0);
    draw_line(0 + cameraPos.x + 1000, 0 + cameraPos.y - 1000, 20 + cameraPos.z, 1000, 1);
    draw_line(0 + cameraPos.x, 0 + cameraPos.y - 1000, 20 + cameraPos.z, 1000, 1);
    draw_line(0 + cameraPos.x, 0 + cameraPos.y - 1000, 20 + cameraPos.z, 1000, 0);
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
    } else {
        for (int i = 0; i < len; i++) {
            float projectedX = (x1) * fov / (fov + z1) + SCREEN_WIDTH / 2;
            float projectedY = (y1 + i) * fov / (fov + z1) + SCREEN_HEIGHT / 2;
            plot_pixel(projectedX, projectedY, 0x0);
        }
    }
}