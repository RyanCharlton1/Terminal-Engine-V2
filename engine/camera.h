#ifndef ENGINE_CAMERA
#define ENGINE_CAMERA

#include "cglm/cglm.h"

typedef struct _camera {
    vec3 pos;
    vec3 dir;
    vec3 up;
} camera;

camera* init_camera();
void    free_camera();

// Get a given camera's view matrix, the matrix is the inverse of the camera's
// movements, manipulating the world around a viewport fixed at (0,0,0)
void camera_view(camera* c, mat4 out);

#endif