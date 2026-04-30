#include "engine/camera.h"

#include <stdio.h>
#include <stdlib.h>

camera* init_camera() {
    camera* out = (camera*)malloc(sizeof(camera));

    if (!out) { printf("malloc failed: init_camera\n"); return NULL; }

    out->pos[0] = 0.0f; out->pos[1] = 0.0f; out->pos[2] =  0.0f;
    out->dir[0] = 0.0f; out->dir[1] = 0.0f; out->dir[2] = -1.0f;
    out->up[0]  = 0.0f; out->up[1]  = 1.0f; out->up[2] =  0.0f;

    return out;
}

void free_camera(camera* c) { 
    free(c);
}

void camera_view(camera* c, mat4 out) {
    vec3 looking_at;

    glm_vec3_add(c->pos, c->dir, looking_at);
    glm_lookat(c->pos, looking_at, c->up, out);
}