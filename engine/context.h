#ifndef ENGINE_CNAVAS
#define ENGINE_CANVAS

#include "engine/colour.h"

#include "cglm/cglm.h"

#define RESET_CURSOR_ESCAPE   "\e[H"
#define DISABLE_CURSOR_ESCAPE "\e[?25l"
#define ENABLE_CURSOR_ESCAPE  "\e[?25h"

typedef struct _context{
    int width, height;

    RGB   *colour_buffer;
    float *depth_buffer;

    // Create a big string and print in one because terminals will drop 
    // trailing whitespace warping the output
    char *char_buffer;
} context;

void init_context(context* cont, int width, int height);
void free_context(context* cont);

void print_context(context* cont);
void print_context16(context* cont);
void print_contextbw(context* cont);

#endif