#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <video.h>
#include <ps2keyboard.h>
#include "draw.h"
#include "pit.h"

int sPosx;
int sPosy;

#define WORLD_LEN 16

typedef uint32_t block;
#define GRASS 0x236533
#define COBBLE 0x808080

block world[WORLD_LEN][WORLD_LEN];

uint32_t last_ticks;

void game_init() {
    print_str("Initializing Minecraft...");
    sPosx = SCREEN_WIDTH / 2 - 30;
    sPosy = SCREEN_HEIGHT / 2 - 30;

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
        sPosx -= deltaTime * speedDiv;
    if (is_key_down('d'))
        sPosx += deltaTime * speedDiv;
    if (is_key_down('w'))
        sPosy -= deltaTime * speedDiv;
    if (is_key_down('s'))
        sPosy += deltaTime * speedDiv;

    draw_square(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xADD8E6);
    
    for (int i = 0; i < WORLD_LEN; i++) {
        for (int j = 0; j < WORLD_LEN; j++) {
            draw_square(i*16+sPosx, j*16+sPosy, i*16+16+sPosx, j*16+16+sPosy, world[i][j]);
        }
            
    }
    last_ticks = ticks;
}