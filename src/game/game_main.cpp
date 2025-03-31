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
    while (!is_key_down(' ')) {}
    clear_screen();
    last_ticks = ticks;
}


void game_loop() {
    int deltaTime = ticks - last_ticks;
    
    // deltaTime is the time per frame, convert this to seconds and invert it for frames per second
    float fps = 1 / ((float)deltaTime / 1000);
    
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
    /*float fov = 68;
    float S = 1 / (tan(fov * M_PI / 360));
    float f = 300;
    float n = 0.01;

    vector3 cameraPos = {0, 0, 1000};
    vector3 rightDir = {1, 0, 0};
    vector3 upDir = {0, -1, 0};
    vector3 fwdDir = {0, 0, -1};

    matrix_m4s view_m = { rightDir[0], rightDir[1], rightDir[2], - dotvec3(rightDir, cameraPos),
                          upDir[0],    upDir[1],    upDir[2],    - dotvec3(upDir, cameraPos),
                          -fwdDir[0],   -fwdDir[1],   -fwdDir[2],   dotvec3(fwdDir, cameraPos),
                          0        ,   0        ,   0,           1};

    matrix_m4s proj_m = {S, 0, 0, 0,
                         0, S, 0, 0,
                         0, 0, -f/(f-n), -1,
                         0, 0,  -(f*n)/(f-n), 1};
    
    vector4 worldPoint1 = {1, 0, 0, 1};
    vector4 worldPoint2 = {1, -10, 0, 1};

    vector4 cameraPoint1 = {0};
    vector4 cameraPoint2 = {0};

    vector4 screenPoint1 = {0};
    vector4 screenPoint2 = {0};


    // Convert from world space to camera space
    multiply_m4sv(view_m, worldPoint1, cameraPoint1);
    multiply_m4sv(view_m, worldPoint2, cameraPoint2);
    
    // Convert from camera space to screen space
    multiply_m4sv(proj_m, cameraPoint1, screenPoint1);
    multiply_m4sv(proj_m, cameraPoint2, screenPoint2);
    
   

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

    if (cameraPoint1[2] > 0 || cameraPoint2[2] > 0)
        return;
        */

    
 //   screenPoint1[0] = 0;
    //draw_line(screenPoint1[0], screenPoint1[1], screenPoint2[0], screenPoint2[1], rgb(129, 37, 175));

   
    

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
    print_float_inplace(cos(0), 0, 0);
    yaw += 0.001 * deltaTime;
    if (yaw > 2 * M_PI)
        yaw -= 2 * M_PI;
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