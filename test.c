#include "engine/context.h"
#include "engine/texture.h"
#include "engine/mesh.h"

#include <stdio.h>
#include <string.h>

#define WIDTH 120
#define HEIGHT 40

int main() {
    context cont;

    init_context(&cont, WIDTH, HEIGHT);
    texture* skull_text = init_texture("burning.png");
    texture* spider_text = init_texture("spider.png");

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

    printf(RESET_CURSOR_ESCAPE);
    printf("\033[8;%d;%dt", HEIGHT, WIDTH); // Linux only

    wipe_depth_buffer(&cont);
    wipe_colour_buffer(&cont);
    
    mat4 proj;
    glm_perspective_default((float)WIDTH / (float)HEIGHT, proj);

    triangle test_tri_proj = transform(proj, &test_tri);
    triangle overlap_tri_proj = transform(proj, &overlap_tri);
    rasterise(&test_tri_proj, &cont, skull_text);
    rasterise(&overlap_tri_proj, &cont, spider_text);

    print_contextbw(&cont);
    print_context16(&cont);
    print_context(&cont);

    free_context(&cont);
    free_texture(skull_text);
    free_texture(spider_text);
    
    return 0;
}