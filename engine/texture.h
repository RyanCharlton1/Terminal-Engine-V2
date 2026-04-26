#ifndef ENGINE_TEXTURE
#define ENGINE_TEXTURE

#include "engine/colour.h"

#include <stdlib.h>

typedef struct _texture {
    unsigned int width, height, channels;

    RGB* data;
} texture;

texture* init_texture(const char* path);
void     free_texture(texture* t);

RGB sample(texture* t, int x, int y);
RGB sampleuv(texture *t, float u, float v);

#endif
