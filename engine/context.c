#include "engine/context.h"
#include "engine/colour.h"

#include <stdlib.h>
#include <string.h>
#include <float.h>

#ifdef __linux__

#include <unistd.h> 

#endif

context* init_context(int width, int height, colour_depth depth) {
    context* cont = (context*)malloc(sizeof(context));

    if (!cont) { printf("malloc failed: init_context\n"); return NULL; }

    cont->width  = width;
    cont->height = height;
    cont->depth  = depth;

    // The max length string to encode a single char, terminator counts as 
    // char itself
    const char max_char[] = "\e[38;2;255;255;255m";

    cont->colour_buffer = (RGB*)malloc(width * height * sizeof(RGB));
    cont->depth_buffer  = (float*)malloc(width * height * sizeof(float));
    cont->char_buffer   = (char*)malloc(width * height * sizeof(max_char) + 1);

    if (!cont->colour_buffer || !cont->depth_buffer || !cont->char_buffer) 
        fprintf(stderr, "malloc failed: init_context\n");

#ifdef _WIN32
    cont->hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

    // TODO: Set terminal size to context resolution

    return cont;
}

void free_context(context* cont) {
    if (cont->colour_buffer) { free(cont->colour_buffer); }
    if (cont->depth_buffer)  { free(cont->depth_buffer);  }
    if (cont->char_buffer)   { free(cont->char_buffer);   }
}

void wipe_buffers(context* cont) {
    wipe_depth_buffer(cont);
    wipe_colour_buffer(cont);
}

void wipe_depth_buffer(context* cont) {
    for (int i = 0; i < cont->height * cont->width; i++)
        cont->depth_buffer[i] = 1.0f;
}

void wipe_colour_buffer(context* cont) {
    memset(cont->colour_buffer, 0, cont->width * cont->height * sizeof(RGB));
}

void print_context(context* cont) {
    switch (cont->depth) {
        case TRUECOLOUR:    print_contexttr(cont); break;
        case REDUCEDCOLOUR: print_context16(cont); break;
        default:            print_contextbw(cont); break;
    }

    // Using printf makes many CLR and win api calls on Windows. Directly using
    // lower level APIs massively increases FPS 
#ifdef _WIN32
    WriteFile(
        cont->hStdout, 
        RESET_CURSOR_ESCAPE, 
        sizeof(RESET_CURSOR_ESCAPE), 
        NULL, 
        NULL);

    WriteFile(
        cont->hStdout, 
        cont->char_buffer, 
        cont->char_buffer_size, 
        NULL, 
        NULL);
#else
    write(STDOUT_FILENO, RESET_CURSOR_ESCAPE, sizeof(RESET_CURSOR_ESCAPE));
    write(STDOUT_FILENO, cont->char_buffer, cont->char_buffer_size);
#endif  
}

void print_contexttr(context* cont) {
    char *char_buffer_ptr = cont->char_buffer;
    cont->char_buffer[0] = '\0';

    for (int i = 0; i < cont->width * cont->height; i++) {
        // Brighter colours will have a fuller ASCII char
        RGB rgb = cont->colour_buffer[i];
        char c = RGB_pixel(rgb); 

        int write_len = sprintf(char_buffer_ptr, "\e[38;2;%hhu;%hhu;%hhum%c",
            rgb.r, rgb.g, rgb.b, c);
           
        char_buffer_ptr += write_len;
    }

    cont->char_buffer_size = char_buffer_ptr - cont->char_buffer;
}

void print_context16(context* cont) {
    char *char_buffer_ptr = cont->char_buffer;
    cont->char_buffer[0] = '\0';

    for (int i = 0; i < cont->width * cont->height; i++) {
        // Brighter colours will have a fuller pixel ASCII char
        RGB rgb = cont->colour_buffer[i];
        char c = RGB_pixel(rgb); 

        colour16 colour = RGB_colour16(rgb);

        int write_len = sprintf(char_buffer_ptr, "\e[%hhum%c", colour, c);

        char_buffer_ptr += write_len;
    }

    cont->char_buffer_size = char_buffer_ptr - cont->char_buffer;
}

void print_contextbw(context* cont) {
    char *char_buffer_ptr = cont->char_buffer;

    for (int i = 0; i < cont->width * cont->height; i++) {
        // Brighter colours will have a fuller pixel ASCII char
        RGB rgb = cont->colour_buffer[i];
        char c = RGB_pixel(rgb); 

        *char_buffer_ptr = c;
        char_buffer_ptr++;
    }

    cont->char_buffer_size = cont->width * cont->height;
}

// Screen space is 0 -> width and 0 -> height with top left as (0,0)
// Inputs are expected to be in space -1 -> 1 
void screenspace(context* cont, vec2 in, vec2 out) {
    out[0] = ( in[0] + 1.0f) / 2.0f * cont->width;
    out[1] = (-in[1] + 1.0f) / 2.0f * cont->height;
}