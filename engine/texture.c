#include "engine/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

texture* init_texture(const char* path) {
    texture *out = (texture*)calloc(1, sizeof(texture));

    if (!out) { 
        fprintf(stderr, "calloc failed: init_texture(%s)\n", path);
        return NULL;
    }

    load_texture(path, out);

    return out;
}

void load_texture(const char *path, texture *t) {
    t->data = (RGB*)stbi_load(
        path, &t->width, &t->height, &t->channels, 3);

    const char* error = stbi_failure_reason();

    if (error) {
        fprintf(stderr, "stbi_load failed: init_texture(%s): %s", path, error);
    }
}

void free_texture(texture* t) {
    if (t->data) { free(t->data); }
    if (t->name) { free(t->name); }
    free(t);
}

RGB sample(texture* t, int x, int y) {
    return t->data[y * t->width + x];
}

RGB sampleuv(texture* t, vec2 uv) {
    return sample(t, (t->width - 1) * uv[0], (t->height - 1) * (1 - uv[1]));
}

