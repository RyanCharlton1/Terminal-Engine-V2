#include "engine/engine.h"
#include "engine/context.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

engine* init_engine(int width, int height, colour_depth depth) {
    engine* e = (engine*)malloc(sizeof(engine));

    if (!e) { printf("malloc failed: init_engine\n"); return NULL; }

    memset(e, 0, sizeof(engine));

    e->current_context = init_context(width, height, depth); 

    if (!e->current_context) { return NULL; }

    printf("\033[8;%d;%dt", height, width); // Resize terminal

    return e;
}

void free_engine(engine* e) {
    if (e->current_context) { free_context(e->current_context); }
    free(e);
}

void start_engine(engine* e) {
    e->init_resources();

    printf(DISABLE_CURSOR_ESCAPE);

    e->running = 1;

    double  frame_time  = 0.0;
    clock_t frame_start = clock();
    while (e) {
        poll_input(e);

        wipe_buffers(e->current_context);

        e->logic(frame_time);
        e->render(e->current_context);
        e->input(frame_time);

        print_context(e->current_context);

        time_t frame_end = clock();

        frame_time = (double)(frame_end - frame_start) / CLOCKS_PER_SEC;

        frame_start = frame_end;
    }

    e->free_resources();

    printf(ENABLE_CURSOR_ESCAPE);
    printf(RESET_COLOUR_ESCAPE); // Reset terminal colour
}

void poll_input(engine* e) {
    
}