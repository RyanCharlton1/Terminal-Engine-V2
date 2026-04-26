#include "engine/context.h"
#include "engine/colour.h"

#include <stdlib.h>
#include <string.h>
#include <float.h>

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
        cont->depth_buffer[i] = FLT_MAX;
}

void wipe_colour_buffer(context* cont) {
    memset(cont->colour_buffer, 0, cont->width * cont->height * sizeof(RGB));
}

void print_context(context* cont) {
    switch (cont->depth) {
        case TRUECOLOUR:    print_contexttr(cont); return;
        case REDUCEDCOLOUR: print_context16(cont); return;
        default:            print_contextbw(cont); return;
    }
}

void print_contexttr(context* cont) {
    char *char_buffer_ptr = cont->char_buffer;

    char buffer[64];
    cont->char_buffer[0] = '\0';

    int n = 0;
    for (int i = 0; i < cont->width * cont->height; i++) {
        // Brighter colours will have a fuller ASCII char
        RGB rgb = cont->colour_buffer[i];
        char c = RGB_pixel(rgb); 

        int write_len = sprintf(char_buffer_ptr, "\e[38;2;%hhu;%hhu;%hhum%c",
            rgb.r, rgb.g, rgb.b, c);
           
        char_buffer_ptr += write_len;
    }

    printf(RESET_CURSOR_ESCAPE); // Set cursor to overwrite last frame
    printf(cont->char_buffer);
}

void print_context16(context* cont) {

    for (int i = 0; i < cont->width * cont->height; i++) {
         if (i % cont->width == 0) { printf("\n"); }
        // Brighter colours will have a fuller pixel ASCII char
        RGB rgb = cont->colour_buffer[i];
        char c = RGB_pixel(rgb); 

        colour16 colour = RGB_colour16(rgb);

        printf("\e[%hhum%c", colour, c);
    }
}

void print_contextbw(context* cont) {

    for (int i = 0; i < cont->width * cont->height; i++) {
        // Brighter colours will have a fuller pixel ASCII char
        RGB rgb = cont->colour_buffer[i];
        char c = RGB_pixel(rgb); 

        printf("%c", c);
    }
}

// Screen space is 0 -> width and 0 -> height with top left as (0,0)
// Inputs are expected to be in space -1 -> 1 
void screenspace(context* cont, vec2 in, vec2 out) {
    out[0] = (in[0] + 1.0f) / 2.0f * cont->width;
    out[1] = (-in[1] + 1.0f) / 2.0f * cont->height;
}