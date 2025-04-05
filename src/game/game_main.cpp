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
#include "projection.h"

int sPosx;
int sPosy;

#define WORLD_LEN 16

typedef uint32_t block;
#define GRASS 0x236533
#define COBBLE 0x808080

block world[WORLD_LEN][WORLD_LEN];

uint32_t last_ticks;

//vector3 cameraPos = {0};

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
    while (!is_key_down(KEY_SPACE)) {}
    clear_screen();
    last_ticks = ticks;
}


void game_loop() {
    int deltaTime = ticks - last_ticks;
    
    // deltaTime is the time per frame, convert this to seconds and invert it for frames per second
    float fps = 1.0 / ((float)deltaTime / 1000.0);
    
    float speedDiv = 0.01;
    if (is_key_down('a'))
        cameraPos[0] += deltaTime * speedDiv;
    if (is_key_down('d'))
        cameraPos[0] -= deltaTime * speedDiv;
    if (is_key_down('w'))
        cameraPos[2] -= deltaTime * speedDiv;
    if (is_key_down('s'))
        cameraPos[2] += deltaTime * speedDiv;
    if (is_key_down('c'))
        cameraPos[1] -= deltaTime * speedDiv;
    if (is_key_down(' '))
        cameraPos[1] += deltaTime * speedDiv;

    draw_square(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xADD8E6);

    vector4 wpoint1 = {0, 0, 0, 1};
    vector4 wpoint2 = {1, 0, 0, 1};
    vector4 wpoint3 = {1, 1, 0, 1};
    vector4 wpoint4 = {0, 1, 0, 1};
    
    vector4 wpoint5 = {0, 0, 1, 1};
    vector4 wpoint6 = {1, 0, 1, 1};
    vector4 wpoint7 = {1, 1, 1, 1};
    vector4 wpoint8 = {0, 1, 1, 1};

    vector4 cpoint1, cpoint2, cpoint3, cpoint4, cpoint6, cpoint5, cpoint7, cpoint8, npoint1, npoint2, npoint3, npoint4, npoint5, npoint6, npoint7, npoint8, spoint1, spoint2, spoint3, spoint4, spoint5, spoint6, spoint7, spoint8;

    world_to_camera_project(wpoint1, cpoint1);
    world_to_camera_project(wpoint2, cpoint2);
    world_to_camera_project(wpoint3, cpoint3);
    world_to_camera_project(wpoint4, cpoint4);
    world_to_camera_project(wpoint5, cpoint5);
    world_to_camera_project(wpoint6, cpoint6);
    world_to_camera_project(wpoint7, cpoint7);
    world_to_camera_project(wpoint8, cpoint8);
    camera_to_ndc_project  (cpoint1, npoint1);
    camera_to_ndc_project  (cpoint2, npoint2);
    camera_to_ndc_project  (cpoint3, npoint3);
    camera_to_ndc_project  (cpoint4, npoint4);
    camera_to_ndc_project  (cpoint5, npoint5);
    camera_to_ndc_project  (cpoint6, npoint6);
    camera_to_ndc_project  (cpoint7, npoint7);
    camera_to_ndc_project  (cpoint8, npoint8);
    if (!(cpoint1[2] > 0 || cpoint2[2] > 0)) {
        ndc_to_screen_project  (npoint1, spoint1);
        ndc_to_screen_project  (npoint2, spoint2);
        ndc_to_screen_project  (npoint3, spoint3);
        ndc_to_screen_project  (npoint4, spoint4);
        ndc_to_screen_project  (npoint5, spoint5);
        ndc_to_screen_project  (npoint6, spoint6);
        ndc_to_screen_project  (npoint7, spoint7);
        ndc_to_screen_project  (npoint8, spoint8);
    
        draw_line(spoint1[0], spoint1[1], spoint2[0], spoint2[1], rgb(248, 42, 42)); // Bottom front
        draw_line(spoint2[0], spoint2[1], spoint3[0], spoint3[1], rgb(248, 42, 42)); // Bottom right
        draw_line(spoint3[0], spoint3[1], spoint4[0], spoint4[1], rgb(248, 42, 42)); // Bottom left
        draw_line(spoint4[0], spoint4[1], spoint1[0], spoint1[1], rgb(248, 42, 42)); // Bottom back
    
        draw_line(spoint5[0], spoint5[1], spoint6[0], spoint6[1], rgb(248, 42, 42)); // Top front
        draw_line(spoint6[0], spoint6[1], spoint7[0], spoint7[1], rgb(248, 42, 42)); // Top right
        draw_line(spoint7[0], spoint7[1], spoint8[0], spoint8[1], rgb(248, 42, 42)); // Top left
        draw_line(spoint8[0], spoint8[1], spoint5[0], spoint5[1], rgb(248, 42, 42)); // Top back
    
        draw_line(spoint1[0], spoint1[1], spoint5[0], spoint5[1], rgb(248, 42, 42)); // Front left vertical
        draw_line(spoint2[0], spoint2[1], spoint6[0], spoint6[1], rgb(248, 42, 42)); // Front right vertical
        draw_line(spoint3[0], spoint3[1], spoint7[0], spoint7[1], rgb(248, 42, 42)); // Back right vertical
        draw_line(spoint4[0], spoint4[1], spoint8[0], spoint8[1], rgb(248, 42, 42)); // Back left vertical    
    }
        
    

    update_viewm();

    print_float_inplace(fps, 0, 0);

    last_ticks = ticks;
    
}


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