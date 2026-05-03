#include "engine/context.h"
#include "engine/texture.h"
#include "engine/mesh.h"
#include "engine/engine.h"

#include <stdio.h>
#include <string.h>

texture* skull_text  = NULL;
texture* spider_text = NULL;

mesh* skull;
mesh* spider;

triangle left_tri = {
    { { -1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
    { { -1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
    { {  1.0f,  1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } }
};

triangle right_tri = {
    { { -1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
    { {  1.0f,  1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
    { {  1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } }
};

void init_resources(engine* self) {
    skull_text  = init_texture("burning.png");
    spider_text = init_texture("spider.png");

    skull  = init_mesh();
    spider = init_mesh();

    skull->model_size = 1;
    skull->tris = &left_tri;
    skull->text = skull_text;

    spider->model_size = 1;
    spider->tris = &right_tri;
    spider->text = spider_text;

    spider->trans.trans[2] = -2.0f;

    skull->trans.trans[2] = -4.0f;
}

void free_resources(engine* self) {
    free_texture(skull_text);
    free_texture(spider_text);

    free_mesh(skull);
    free_mesh(spider);
}

void logic(double frame_time, engine* self) {
    camera *cam = self->current_camera;

    if (self->keys['W']) { glm_vec3_muladds(cam->dir,  frame_time, cam->pos); }
    if (self->keys['S']) { glm_vec3_muladds(cam->dir, -frame_time, cam->pos); }
    if (self->keys['A']) { glm_vec3_rotate(cam->dir,  frame_time, cam->up); }
    if (self->keys['D']) { glm_vec3_rotate(cam->dir, -frame_time, cam->up); }
    if (self->keys['K']) { self->running = 0; } // Quit

    skull->trans.rot[1] += frame_time;
}

void render(engine* self) {
    draw_mesh(skull, self->current_context, self->current_camera);
    draw_mesh(spider, self->current_context, self->current_camera);
}

int main() {
    print_target_os();
    engine* e = init_engine(WIDTH, HEIGHT, TRUECOLOUR);

    e->init_resources = init_resources; 
    e->free_resources = free_resources; 
    e->logic  = logic; 
    e->render = render; 

    start_engine(e);

    free_engine(e);
}