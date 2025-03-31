#ifndef PROJECTION_H
#define PROJECTION_H

#include <matrix.h>
#include <math.h>

constexpr  float fov = 68;

// precomputed value for tan(fov in radians)
constexpr  float aspect_ratio = 4.0f / 3.0f;
constexpr  float tanfov = 0.67450851684;
constexpr  float S = 1 / (tanfov );
constexpr  float f = 300;
constexpr  float n = 0.1;



constexpr matrix_m4s proj_m = { S / aspect_ratio,  0,  0,   0,
                                0,  S,  0,   0,
                                0,  0,  -(f+n)/(f-n), -1,
                                0,  0,  -2*f*n/(f-n),  1};



static vector3 cameraPos = {0, 0, 10};
constexpr  vector3 rightDir = {-1, 0, 0};
constexpr  vector3 upDir = {0, 1, 0};
constexpr  vector3 fwdDir = {0, 0, -1};

static float pitch = 0;
static float yaw = 0;
static float roll = 0;
static matrix_m4s rot_m = {  1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1};

static matrix_m4s view_m = { rightDir[0], rightDir[1], rightDir[2], - dotvec3(rightDir, cameraPos),
upDir[0],    upDir[1],    upDir[2],    - dotvec3(upDir, cameraPos),
-fwdDir[0],   -fwdDir[1],   -fwdDir[2],   dotvec3(fwdDir, cameraPos),
0        ,   0        ,   0,           1};


static inline void update_viewm() {
    set_index_m4s(view_m, 3, 0, -dotvec3(rightDir, cameraPos));
    set_index_m4s(view_m, 3, 1, - dotvec3(upDir, cameraPos));
    set_index_m4s(view_m, 3, 2, dotvec3(fwdDir, cameraPos));

    set_index_m4s(rot_m, 0, 0, cos(yaw)*cos(roll) - sin(pitch)*sin(yaw)*sin(roll));
    set_index_m4s(rot_m, 0, 1, -cos(yaw)*sin(roll) - sin(pitch)*sin(yaw)*cos(roll));
    set_index_m4s(rot_m, 0, 2, sin(pitch)*cos(yaw));
    set_index_m4s(rot_m, 1, 0, cos(pitch)*sin(roll));
    set_index_m4s(rot_m, 1, 1, cos(pitch)*cos(roll));
    set_index_m4s(rot_m, 1, 2, -sin(pitch));
    set_index_m4s(rot_m, 2, 0, sin(yaw)*cos(roll) + sin(pitch)*cos(yaw)*sin(roll));
    set_index_m4s(rot_m, 2, 1, -sin(yaw)*sin(roll) + sin(pitch)*cos(yaw)*cos(roll));
    set_index_m4s(rot_m, 2, 2, cos(pitch)*cos(yaw));


        
}
// transforms the point worldPoint into camera space in cameraPoint
static inline void world_to_camera_project(const vector4 worldPoint, vector4 cameraPoint) { 
    matrix_m4s rotated_m;
    multiply_m4s((float*)view_m, rot_m, rotated_m);
    multiply_m4sv((float*)rotated_m, worldPoint, cameraPoint); 
}
static inline void camera_to_ndc_project(const vector4 cameraPoint, vector4 ndcPoint) { 

    multiply_m4sv((float*)proj_m, cameraPoint, ndcPoint); 
    ndcPoint[0] = ndcPoint[0] / ndcPoint[3];
    ndcPoint[1] = ndcPoint[1] / ndcPoint[3];
    ndcPoint[2] = ndcPoint[2] / ndcPoint[3]; 
}
static inline void ndc_to_screen_project(const vector4 ndcPoint, vector2 screenPoint) { 
    screenPoint[0] = (ndcPoint[0] / ndcPoint[3] + 1) / 2 * SCREEN_WIDTH;

    screenPoint[1] = (1 - ndcPoint[1] / ndcPoint[3]) / 2 * SCREEN_HEIGHT;
 }

#endif