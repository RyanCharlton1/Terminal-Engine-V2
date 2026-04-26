#ifndef ENGINE_TRANSFORM
#define ENGINE_TRANSFORM

#include "cglm/cglm.h"

typedef struct _transform {
    vec3 rot;
    vec3 scale;
    vec3 trans;
} transform;

void transform_mat4(mat4 mat, transform* t);

#endif