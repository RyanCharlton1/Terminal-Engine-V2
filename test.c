#include "engine/context.h"
#include "engine/texture.h"
#include "engine/mesh.h"
#include "engine/engine.h"

#include <stdio.h>
#include <string.h>

#define WIDTH 120
#define HEIGHT 40

texture* skull_text  = NULL;
texture* spider_text = NULL;

triangle test_tri = {
    { { -1.0f, -1.0f, -4.0f, 1.0f }, { 0.0f, 1.0f } },
    { { -1.0f,  1.0f, -4.0f, 1.0f }, { 0.0f, 0.0f } },
    { {  1.0f,  1.0f, -4.0f, 1.0f }, { 1.0f, 0.0f } }
};

triangle overlap_tri = {
    { { -1.0f,  1.0f, -2.0f, 1.0f }, { 0.0f, 0.0f } },
    { {  1.0f,  1.0f, -2.0f, 1.0f }, { 1.0f, 0.0f } },
    { {  1.0f, -1.0f, -2.0f, 1.0f }, { 1.0f, 1.0f } }
};

void init_resources(engine* self) {
    skull_text  = init_texture("burning.png");
    spider_text = init_texture("spider.png");
}

void free_resources(engine* self) {
    free_texture(skull_text);
    free_texture(spider_text);
}

void logic(double frame_time, engine* self) {
    if (self->keys['W']) { self->current_camera->z -= 1.0 * frame_time; }
    if (self->keys['S']) { self->current_camera->z += 1.0 * frame_time; }
    if (self->keys['A']) { self->current_camera->yaw += 1.0 * frame_time; }
    if (self->keys['D']) { self->current_camera->yaw -= 1.0 * frame_time; }
    if (self->keys['K']) { self->running = 0; } // Quit
    // self->current_camera->yaw += 1.0 * frame_time;
}

void render(engine* self) {
    mat4 proj, view;

    glm_perspective_default((float)WIDTH / (float)HEIGHT, proj);

    camera_view(self->current_camera, view);

    triangle test_tri_view    = transform(view, &test_tri);
    triangle overlap_tri_view = transform(view, &overlap_tri);

    triangle test_tri_proj    = transform(proj, &test_tri_view);
    triangle overlap_tri_proj = transform(proj, &overlap_tri_view);

    rasterise(&test_tri_proj, self->current_context, skull_text);
    rasterise(&overlap_tri_proj, self->current_context, spider_text);
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