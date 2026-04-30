#include "engine/camera.h"

#include <stdio.h>
#include <stdlib.h>

camera* init_camera() {
    camera* out = (camera*)malloc(sizeof(camera));

    if (!out) { printf("malloc failed: init_camera\n"); return NULL; }

    return out;
}

void free_camera(camera* c) { 
    free(c);
}

void camera_view(camera* c, mat4 out) {
    vec3 inverse_rot   = { -c->pitch, -c->yaw, -c->roll };
    vec3 inverse_trans = { -c->x,     -c->y,   -c->z    };

    mat4 trans_m, rot_m;
    glm_translate_make(trans_m, inverse_trans);
    glm_euler_yxz((float*)inverse_rot, rot_m); 
    glm_mat4_mul(trans_m, rot_m, out);
}