#include "engine/context.h"
#include "engine/texture.h"

#include <stdio.h>

#define WIDTH 60
#define HEIGHT 40

int main() {
    context cont;

    init_context(&cont, WIDTH, HEIGHT);
    texture* t = init_texture("burning.png");

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            cont.colour_buffer[y * cont.width + x] = 
                sampleuv(t, (float)x / (float)(WIDTH - 1), (float)y / (float)(HEIGHT - 1));
        }
    }

    printf(RESET_CURSOR_ESCAPE);
    printf("\033[8;%d;%dt", HEIGHT, WIDTH); // Linux only

    print_contextbw(&cont);
    print_context16(&cont);
    print_context(&cont);

    free_texture(t);
    
    return 0;
}