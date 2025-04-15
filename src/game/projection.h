#ifndef PROJECTION_H
#define PROJECTION_H

#include <matrix.h>
#include <math.h>

// To speed up the calculations, assume we know the screen width/height at compile-time
constexpr int expected_screen_width = 640;
constexpr int expected_screen_height = 480;

constexpr float near_plane = 0.1f;
constexpr float far_plane = 1000.0f;

// precalculate $tan(\frac{\theta}{2})$ where \theta is the fov (this is for fov = 68)
constexpr float tanfov = 0.6751f;

constexpr float aspect_ratio = float(expected_screen_width) / float(expected_screen_height);


constexpr matrix_m4s projection_matrix = {
    (1.0f/aspect_ratio) * (1.0f / tanfov), 0, 0, 0,
    0, 1.0f / tanfov, 0, 0,
    0, 0, far_plane / (far_plane - near_plane), 1,
    0, 0, (-1 * far_plane * near_plane) / (far_plane - near_plane), 0
};


#endif