#ifndef ENGINE_ENGINE
#define ENGINE_ENGINE

#include "engine/context.h"
#include "engine/input.h"

typedef struct _engine {
    context* current_context;

    key_state keys[KEY_COUNT];

    int running;

    void (*init_resources)();
    void (*free_resources)();
    void (*logic)(double);
    void (*render)(context*);
    void (*input)(double);
} engine;

engine* init_engine(int width, int height, colour_depth depth);
void    free_engine(engine* e);

void start_engine(engine* e);

void poll_input(engine* e);

#endif