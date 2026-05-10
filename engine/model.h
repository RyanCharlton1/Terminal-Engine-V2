#ifndef ENGINE_MODEL
#define ENGINE_MODEL

#include "engine/mesh.h"
#include "engine/transform.h"

// A composite of meshes. A single mesh will have a single texture so complex
// models require multiple meshes
typedef struct _model {
    mesh    **meshes;   // List of pointers to dynamically allocated meshes
    texture **textures; // Model textures, multiple meshes use single texture

    unsigned int mesh_count;
    unsigned int texture_count;

    transform trans;
} model;

model* init_model();
void   free_model(model* m);

// Load a model from OBJ/MTL
model* load_model(const char *path);

void load_mtl(const char *path, model* m);

void draw_model(model *m, context *cont, camera *cam);

#endif