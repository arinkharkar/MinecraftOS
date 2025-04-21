#ifndef PROJECTION_H
#define PROJECTION_H

#include <matrix.h>
#include <math.h>
/*
 * https://github.com/OneLoneCoder/Javidx9/tree/master/ConsoleGameEngine/BiggerProjects/Engine3D credit to Javidx9 for helping with the projection code, really helped me out
*/

// To speed up the calculations, assume we know the screen width/height at compile-time
#define expected_screen_width 640
#define expected_screen_height 480

#define near_plane 0.1f
#define far_plane 1000.0f

// precalculate $tan(\frac{\theta}{2})$ where \theta is the fov (this is for fov = 68)
#define tanfov 0.6751f

#define aspect_ratio ((float)(expected_screen_width) / (float)(expected_screen_height))

matrix_m4s projection_matrix = {
    (1.0f/aspect_ratio) * (1.0f / tanfov), 0, 0, 0,
    0, 1.0f / tanfov, 0, 0,
    0, 0, far_plane / (far_plane - near_plane), 1,
    0, 0, (-1 * far_plane * near_plane) / (far_plane - near_plane), 0
};

static matrix_m4s rot_matrix_z = {0};

static matrix_m4s rot_matrix_x = {0};

#endif