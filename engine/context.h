#ifndef ENGINE_CANVAS
#define ENGINE_CANVAS

#include "engine/colour.h"

#include "cglm/cglm.h"

#ifdef _WIN32

#include <windows.h>

#endif

#define RESET_COLOUR_ESCAPE   "\e[0m"
#define RESET_CURSOR_ESCAPE   "\e[H"
#define ENABLE_CURSOR_ESCAPE  "\e[?25h"
#define DISABLE_CURSOR_ESCAPE "\e[?25l"

#define WIDTH  120
#define HEIGHT 40

typedef enum {
    TRUECOLOUR,
    REDUCEDCOLOUR,
    BWCOLOUR
} colour_depth;

typedef struct _context{
    int width, height;
    colour_depth depth;

    RGB   *colour_buffer;
    float *depth_buffer;

    // Create a big string and print in one because terminals drop trailing
    // whitespace warping the output
    char *char_buffer;
    int char_buffer_size; // Amount of char_buffer used

#ifdef _WIN32
    HANDLE hStdout;
#endif
} context;

context* init_context(int width, int height, colour_depth depth);
void     free_context(context* cont);

void wipe_buffers(context* cont);
void wipe_depth_buffer(context* cont);
void wipe_colour_buffer(context* cont);

// Print colour buffer to char buffer then char buffer to terminal
void print_context(context* cont);
// Print colour buffer to char buffer for each colour depth
void print_contexttr(context* cont);
void print_context16(context* cont);
void print_contextbw(context* cont);

void screenspace(context* cont, vec2 in, vec2 out);

#endif