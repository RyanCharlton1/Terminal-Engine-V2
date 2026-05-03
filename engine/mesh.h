#ifndef ENGINE_MESH
#define ENGINE_MESH

#include "engine/context.h"
#include "engine/texture.h"
#include "engine/transform.h"
#include "engine/camera.h"

#include "cglm/cglm.h"

typedef struct _vertex {
    vec4 pos;   // Position in 3D space with 4th component scaling
    vec2 uv;    // Texture coordinate
} vertex;

typedef struct _triangle {
    vertex left;
    vertex top;
    vertex right;
} triangle;

triangle apply_transform(mat4 trans, triangle* src);

// Find the distance of p from edge ab propotional to |ab|, the sign of which
// tells what side of the edge p falls on 
float implicit_line(vec2 a, vec2 b, vec2 p);

// Barycentric space is in reference to a triangle with coordinates (a,b,c)
// representing a weighting of each of the triangles vertices, summing to 1.
// These weights can be used for interpolation of depth and texture coords.
// All weights being positive means point p falls within the triangle and needs
// drawing.
// At this point all coordinates will be 2 dimensional, in screen space
void barycentric(
    vec2 p, vec2 left, vec2 top, vec2 right, 
    float* alpha, float *beta, float* gamma);
    
// Simply check all values are positive
int inside_triangle(float alpha, float beta, float gamma);

void rasterise(triangle* t, context* cont, texture* text);

// model and texture are shared resources that need to managed separately
// multiple meshes may reuse the same textures or models
typedef struct _mesh {
    triangle* tris;
    texture*  text;

    unsigned int model_size;

    transform trans;
} mesh;

mesh* init_mesh();
void  free_mesh(mesh* m);

// Apply transformations to mesh and draw to cont
void draw_mesh(mesh *m, context *cont, camera* cam);

#endif