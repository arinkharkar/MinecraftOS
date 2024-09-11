#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <video.h>
#include <ps2keyboard.h>
#include "draw.h"

int sPosx;
int sPosy;

void game_init() {
    print_str("Initializing Minecraft...");
    sPosx = SCREEN_WIDTH / 2;
    sPosy = SCREEN_HEIGHT / 2;

    print_str("Ready, press Space to start");
    swap();
    while (!is_key_down(' ')) {}
    clear_screen();
}


void game_loop() {
    if (is_key_down('a'))
        sPosx -= 10;
    if (is_key_down('d'))
        sPosx += 10;
    if (is_key_down('w'))
        sPosy -= 10;
    if (is_key_down('s'))
        sPosy += 10;

    draw_square(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xADD8E6);
    
    draw_square(sPosx, sPosy, sPosx + 100, sPosy + 100, 0x236533);
}