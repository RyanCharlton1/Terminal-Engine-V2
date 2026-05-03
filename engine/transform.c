#include "engine/transform.h"

void transform_mat4(mat4 mat, transform* t) {
    glm_translate_make(mat, t->trans);
    glm_rotate_x(mat, t->rot[0], mat);
    glm_rotate_y(mat, t->rot[1], mat);
    glm_rotate_z(mat, t->rot[2], mat);
    glm_scale(mat, t->scale);
}