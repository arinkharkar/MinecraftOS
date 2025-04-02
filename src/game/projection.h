#ifndef PROJECTION_H
#define PROJECTION_H

#include <matrix.h>
#include <math.h>

constexpr  float fov = 68;


constexpr  float aspect_ratio = 680.0f / 480.0f;
// precomputed value for tan(fov in radians)
constexpr  float tanfov = 1;
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
static matrix_m4s rot_x = {  1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1};

static matrix_m4s rot_y = { 1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1};

static matrix_m4s rot_z = { 1, 0, 0, 0,
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

    set_index_m4s(rot_z, 0, 0, cos(roll));
    set_index_m4s(rot_z, 0, 1, -sin(roll));
    set_index_m4s(rot_z, 0, 2, 0);
    
    set_index_m4s(rot_z, 1, 0, sin(roll));
    set_index_m4s(rot_z, 1, 1, cos(roll));
    set_index_m4s(rot_z, 1, 2, 0);

    set_index_m4s(rot_z, 2, 0, 0);
    set_index_m4s(rot_z, 2, 1, 0);
    set_index_m4s(rot_z, 2, 2, 1);

    set_index_m4s(rot_x, 0, 0, 1);
    set_index_m4s(rot_x, 0, 1, 0);
    set_index_m4s(rot_x, 0, 2, 0);

    set_index_m4s(rot_x, 1, 0, 0);
    set_index_m4s(rot_x, 1, 1, cos(pitch));
    set_index_m4s(rot_x, 1, 2, -sin(pitch));

    set_index_m4s(rot_x, 2, 0, 0);
    set_index_m4s(rot_x, 2, 1, sin(pitch));
    set_index_m4s(rot_x, 2, 2, cos(pitch));


    set_index_m4s(rot_y, 0, 0, cos(yaw));
    set_index_m4s(rot_y, 0, 1, 0);
    set_index_m4s(rot_y, 0, 2, sin(yaw));

    set_index_m4s(rot_y, 1, 0, 0);
    set_index_m4s(rot_y, 1, 1, 1);
    set_index_m4s(rot_y, 1, 2, 0);

    set_index_m4s(rot_y, 2, 0, -sin(yaw));
    set_index_m4s(rot_y, 2, 1, 0);
    set_index_m4s(rot_y, 2, 2, cos(yaw));
        
}
// transforms the point worldPoint into camera space in cameraPoint
static inline void world_to_camera_project(const vector4 worldPoint, vector4 cameraPoint) { 
    matrix_m4s rotated_m, rotated_mtemp;
    multiply_m4s(rot_y, rot_x, rotated_mtemp);
    multiply_m4s(rotated_mtemp, rot_z, rotated_m);
    multiply_m4s((float*)view_m, rotated_m, rotated_mtemp);

    multiply_m4sv((float*)rotated_mtemp, worldPoint, cameraPoint); 
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