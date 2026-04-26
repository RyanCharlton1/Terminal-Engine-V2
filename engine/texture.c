#include "engine/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

texture* init_texture(const char* path) {
    texture *out = (texture*)malloc(sizeof(texture));

    if (!out) { 
        fprintf(stderr, "malloc failed: init_texture(%s)\n", path);
        return NULL;
    }

    out->data = (RGB*)stbi_load(
        path, &out->width, &out->height, &out->channels, 3);

    const char* error = stbi_failure_reason();

    if (error) {
        if (out) { free(out); }

        fprintf(stderr, "stbi_load failed: init_texture(%s): %s", path, error);
        return NULL;
    }

    return out;
}

void free_texture(texture* t) {
    if (t->data) { free(t->data); }
    free(t);
}

RGB sample(texture* t, int x, int y) {
    return t->data[y * t->width + x];
}

RGB sampleuv(texture* t, float u,  float v) {
    return sample(t, (t->width - 1) * u, (t->height - 1) * v);
}

