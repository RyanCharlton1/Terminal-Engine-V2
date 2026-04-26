#include "engine/context.h"
#include "engine/texture.h"
#include "engine/mesh.h"

#include <stdio.h>
#include <string.h>

#define WIDTH 60
#define HEIGHT 40

int main() {
    context cont;

    init_context(&cont, WIDTH, HEIGHT);
    texture* t = init_texture("burning.png");

    triangle test_tri = {
        { { -1.0f, -1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        { { -1.0f,  1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  1.0f,  1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }
    };

    printf(RESET_CURSOR_ESCAPE);
    printf("\033[8;%d;%dt", HEIGHT, WIDTH); // Linux only

    wipe_depth_buffer(&cont);
    wipe_colour_buffer(&cont);
    rasterise(&test_tri, &cont, t);

    print_contextbw(&cont);
    print_context16(&cont);
    print_context(&cont);

    free_context(&cont);
    free_texture(t);
    
    return 0;
}