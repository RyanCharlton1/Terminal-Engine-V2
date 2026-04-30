#ifndef ENGINE_ENGINE
#define ENGINE_ENGINE

#include "engine/context.h"
#include "engine/input.h"
#include "engine/camera.h"

#ifdef __linux__
#include <linux/input.h>
#include <termios.h>

int find_keyboard_fd();
static int keyboard_fd;

static struct termios original_terminal, altered_terminal;
#endif


typedef struct _engine {
    context* current_context;
    camera*  current_camera;

    key_state keys[KEY_COUNT];

    int running;

    void (*init_resources)(struct _engine*);
    void (*free_resources)(struct _engine*);
    void (*logic) (double, struct _engine*);
    void (*render)(struct _engine*);
} engine;

engine* init_engine(int width, int height, colour_depth depth);
void    free_engine(engine* e);

void start_engine(engine* e);

void poll_input(engine* e);

void print_target_os();

void config_terminal();
void restore_terminal();

#endif