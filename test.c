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

void init_resources() {
    skull_text  = init_texture("burning.png");
    spider_text = init_texture("spider.png");
}

void free_resources() {
    free_texture(skull_text);
    free_texture(spider_text);
}

void logic(double) {}

void render(context* cont) {
    mat4 proj;
    glm_perspective_default((float)WIDTH / (float)HEIGHT, proj);

    triangle test_tri_proj = transform(proj, &test_tri);
    triangle overlap_tri_proj = transform(proj, &overlap_tri);
    rasterise(&test_tri_proj, cont, skull_text);
    rasterise(&overlap_tri_proj, cont, spider_text);
}

void input(double) {}

int main() {
    engine* e = init_engine(WIDTH, HEIGHT, TRUECOLOUR);

    e->init_resources = init_resources; 
    e->free_resources = free_resources; 
    e->logic = logic; 
    e->render = render; 
    e->input = input;

    start_engine(e);

    free_engine(e);
}

// int main() {
//     context* cont = init_context(WIDTH, HEIGHT, TRUECOLOUR);
//     texture* skull_text = init_texture("burning.png");
//     texture* spider_text = init_texture("spider.png");

//     triangle test_tri = {
//         { { -1.0f, -1.0f, -4.0f, 1.0f }, { 0.0f, 1.0f } },
//         { { -1.0f,  1.0f, -4.0f, 1.0f }, { 0.0f, 0.0f } },
//         { {  1.0f,  1.0f, -4.0f, 1.0f }, { 1.0f, 0.0f } }
//     };

//     triangle overlap_tri = {
//         { { -1.0f,  1.0f, -2.0f, 1.0f }, { 0.0f, 0.0f } },
//         { {  1.0f,  1.0f, -2.0f, 1.0f }, { 1.0f, 0.0f } },
//         { {  1.0f, -1.0f, -2.0f, 1.0f }, { 1.0f, 1.0f } }
//     };

//     printf(RESET_CURSOR_ESCAPE);

//     wipe_depth_buffer(cont);
//     wipe_colour_buffer(cont);
    
//     mat4 proj;
//     glm_perspective_default((float)WIDTH / (float)HEIGHT, proj);

//     triangle test_tri_proj = transform(proj, &test_tri);
//     triangle overlap_tri_proj = transform(proj, &overlap_tri);
//     rasterise(&test_tri_proj, &cont, skull_text);
//     rasterise(&overlap_tri_proj, &cont, spider_text);

//     print_contextbw(cont);
//     print_context16(cont);
//     print_context(cont);

//     free_context(cont);
//     free_texture(skull_text);
//     free_texture(spider_text);
    
//     return 0;
// }