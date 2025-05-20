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

uint32_t last_ticks;


void game_init() {
    print_str("Initializing Minecraft...");


    print_str("Ready, press Space to start");
    swap();
    while (!is_key_down(KEY_SPACE)) {}
    clear_screen();
    last_ticks = ticks_passed;
}


void game_loop() {
    float fTheta = (float)(ticks_passed) / 1000.0f;
    // calculate the rotation matrix
    rot_matrix_z[0] = cos(fTheta);
    rot_matrix_z[1] = sin(fTheta);
    rot_matrix_z[4] = -sin(fTheta);
    rot_matrix_z[5] = cos(fTheta);
    rot_matrix_z[10] = 1;
    rot_matrix_z[15] = 1;

    // Rotation X
    rot_matrix_x[0] = 1;
    rot_matrix_x[5] = cos(fTheta * 0.5f);
    rot_matrix_x[6] = sin(fTheta * 0.5f);
    rot_matrix_x[9] = -sin(fTheta * 0.5f);
    rot_matrix_x[10] = cos(fTheta * 0.5f);
    rot_matrix_x[15] = 1;


    int deltaTime = ticks_passed - last_ticks;
    
    // deltaTime is the time per frame, convert this to seconds and invert it for frames per second
    float fps = 1.0 / ((float)deltaTime / 1000.0);
    last_ticks = ticks_passed;
    draw_square(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, rgb(2, 199, 202));


    // iterate through each triangle and draw it
    for (int i = 0; i < CUBE_NUM_TRIS; i++) {
        vector3 projected_triangle[3];
        vector3 translated_triangle[3];
        vector3 tri_rotated_z[3];
        vector3 tri_rotated_x[3];

        multiply_v3m4((matrix_f)rot_matrix_z, cube_triangle_verticies[i][0], tri_rotated_z[0]);
        multiply_v3m4((matrix_f)rot_matrix_z, cube_triangle_verticies[i][1], tri_rotated_z[1]);
        multiply_v3m4((matrix_f)rot_matrix_z, cube_triangle_verticies[i][2], tri_rotated_z[2]);

        multiply_v3m4((matrix_f)rot_matrix_x, tri_rotated_z[0], tri_rotated_x[0]);
        multiply_v3m4((matrix_f)rot_matrix_x, tri_rotated_z[1], tri_rotated_x[1]);
        multiply_v3m4((matrix_f)rot_matrix_x, tri_rotated_z[2], tri_rotated_x[2]);

        memcpy(translated_triangle, tri_rotated_x, sizeof(float)*3*3);

        translated_triangle[0][2] += 3.0f;
        translated_triangle[1][2] += 3.0f;
        translated_triangle[2][2] += 3.0f;


        multiply_v3m4((matrix_f)projection_matrix, translated_triangle[0], projected_triangle[0]);
        multiply_v3m4((matrix_f)projection_matrix, translated_triangle[1], projected_triangle[1]);
        multiply_v3m4((matrix_f)projection_matrix, translated_triangle[2], projected_triangle[2]);

        projected_triangle[0][0] += 1.0f; projected_triangle[0][1] += 1.0f;
        projected_triangle[1][0] += 1.0f; projected_triangle[1][1] += 1.0f;
        projected_triangle[2][0] += 1.0f; projected_triangle[2][1] += 1.0f;

        projected_triangle[0][0] *= 0.5f * expected_screen_width; projected_triangle[0][1] *= 0.5f * expected_screen_height;
        projected_triangle[1][0] *= 0.5f * expected_screen_width; projected_triangle[1][1] *= 0.5f * expected_screen_height;
        projected_triangle[2][0] *= 0.5f * expected_screen_width; projected_triangle[2][1] *= 0.5f * expected_screen_height;

        draw_triangle(projected_triangle[0][0], projected_triangle[0][1], projected_triangle[1][0], projected_triangle[1][1], projected_triangle[2][0], projected_triangle[2][1], rgb(255, 0, 0));
    }



    print_float_inplace(fps, 0, 0);

    
    
}