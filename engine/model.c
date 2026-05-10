#include "engine/model.h"

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

#include <windows.h>

#else

#include <linux/limits.h>

#endif

model* init_model() {
    model* out = (model*)calloc(1, sizeof(model));

    if (!out) { fprintf(stderr, "calloc failed: init_model\n"); return NULL; }

    out->trans.scale[0] = 1.0f;
    out->trans.scale[1] = 1.0f;
    out->trans.scale[2] = 1.0f;

    return out;
}

void free_model(model* m) {
    for (int i = 0; i < m->mesh_count; i++) { free_mesh(m->meshes[i]); }

    if (m->meshes) { free(m->meshes); }

    for (int i = 0; i < m->texture_count; i++) 
        free_texture(m->textures[i]);
    
    if (m->textures) { free(m->textures); }

    free(m);
}

model* load_model(const char *path) {
    model* out = init_model();

    FILE *fp = fopen(path, "r");

    if (fp == NULL) { fprintf(stderr, "failed to fopen %s\n", path); }

    int path_len = strlen(path);
    // Get directory path
    char dir[PATH_MAX];
    strcpy(dir, path);
    char *end = dir + path_len;
    while (*end != '/') { end--; }
    end[1] = '\0';

    vec3 *positions   = NULL;
    vec2 *text_coords = NULL;

    unsigned int pos_count = 0;
    unsigned int uv_count  = 0;

    mesh *current_mesh = NULL;

    char line_buffer[0x400];
    while (fgets(line_buffer, sizeof(line_buffer), fp)) {
        char *opcode = strtok(line_buffer, " ");

        if (!strcmp(opcode, "mtllib")) {
            char mtl_file[PATH_MAX];

            strcpy(mtl_file, dir);
            strcat(mtl_file, strtok(NULL, "\n")); // Append rest of line

            load_mtl(mtl_file, out);
        }
        else if (!strcmp(opcode, "usemtl")) {
            char *mat_name = strtok(NULL, "\n");

            current_mesh = init_mesh();
            out->mesh_count++;
            
            out->meshes = 
                (mesh**)realloc(out->meshes, out->mesh_count * sizeof(mesh*));
            
            out->meshes[out->mesh_count - 1] = current_mesh;

            int index = 0;
            for (;strcmp(out->textures[index]->name, mat_name); index++) {}

            current_mesh->text = out->textures[index];
        }
        else if (!strcmp(opcode, "v")) {
            pos_count++;

            positions = (vec3*)realloc(positions, pos_count * sizeof(vec3));

            positions[pos_count - 1][0] = atof(strtok(NULL, " "));
            positions[pos_count - 1][1] = atof(strtok(NULL, " "));
            positions[pos_count - 1][2] = atof(strtok(NULL, "\n"));
        }
        else if (!strcmp(opcode, "vt")) {
            uv_count++;

            text_coords = (vec2*)realloc(text_coords, uv_count * sizeof(vec2));

            float uvx = atof(strtok(NULL, " "));
            float uvy = atof(strtok(NULL, "\n"));

            // uvs aren't always between 0.0f and 1.0f
            if (uvx < 0.0f || uvx > 1.0f) { uvx = fmod(uvx, 1.0f); }
            if (uvy < 0.0f || uvy > 1.0f) { uvy = fmod(uvy, 1.0f); }

            if (uvx < 0.0f) { uvx += 1.0f; }
            if (uvy < 0.0f) { uvy += 1.0f; }

            text_coords[uv_count - 1][0] = uvx;
            text_coords[uv_count - 1][1] = uvy;
        }
        else if (!strcmp(opcode, "f")) {
            int n = ++current_mesh->mesh_size;

            current_mesh->tris = 
                (triangle*)realloc(current_mesh->tris, n * sizeof(triangle));

            triangle *current_tri = 
                &current_mesh->tris[current_mesh->mesh_size - 1];
                
            unsigned int pos_index = atoi(strtok(NULL, "/")) - 1; // 1 based
            memcpy(current_tri->left.pos, positions[pos_index], sizeof(vec3));

            current_tri->left.pos[3] = 1.0f; // Init w component 

            unsigned int uv_index = atoi(strtok(NULL, "/")) - 1; // 1 based
            memcpy(current_tri->left.uv, text_coords[uv_index], sizeof(vec2));

            strtok(NULL, " ");  // Ignore normals

            pos_index = atoi(strtok(NULL, "/")) - 1; // 1 based
            memcpy(current_tri->top.pos, positions[pos_index], sizeof(vec3));

            current_tri->top.pos[3] = 1.0f; // Init w component

            uv_index = atoi(strtok(NULL, "/")) - 1; // 1 based
            memcpy(current_tri->top.uv, text_coords[uv_index], sizeof(vec2));

            strtok(NULL, " ");  // Ignore normals

            pos_index = atoi(strtok(NULL, "/")) - 1; // 1 based
            memcpy(current_tri->right.pos, positions[pos_index], sizeof(vec3));

            current_tri->right.pos[3] = 1.0f; // Init w component

            uv_index = atoi(strtok(NULL, "/")) - 1; // 1 based
            memcpy(current_tri->right.uv, text_coords[uv_index], sizeof(vec2));

            strtok(NULL, " ");  // Ignore normals
        }
    }

    if (positions)   { free(positions);   }
    if (text_coords) { free(text_coords); }

    fclose(fp);

    return out;
}

void load_mtl(const char *path, model* m) {
    FILE *fp = fopen(path, "r");

    if (fp == NULL) { fprintf(stderr, "failed to fopen %s\n", path); }

    int path_len = strlen(path);
    // Get directory path
    char dir[PATH_MAX];
    strcpy(dir, path);
    char *end = dir + path_len;
    while (*end != '/') { end--; }
    end[1] = '\0';

    texture *current = NULL;

    char line_buffer[0x400];
    while (fgets(line_buffer, sizeof(line_buffer), fp)) {
        char *opcode = strtok(line_buffer, " ");

        if (!strcmp(opcode, "newmtl")) {
            char *mat_name = strtok(NULL, "\n");
            
            current = (texture*)calloc(1, sizeof(texture));
            
            if (!current) { fprintf(stderr, "calloc failed: load_mtl\n"); }

            current->name = (char*)malloc(strlen(mat_name) + 1);

            if (!current) { fprintf(stderr, "malloc failed: load_mtl\n"); }

            strcpy(current->name, mat_name);

            m->texture_count++;

            m->textures = (texture**)realloc(
                m->textures, m->texture_count * sizeof(texture*));
            
            m->textures[m->texture_count - 1] = current;
        }
        else if (!strcmp(opcode, "map_Kd")) {
            char *texture_name = strtok(NULL, "\n");
            
            char texture_path[PATH_MAX];
            strcpy(texture_path, dir);
            strcat(texture_path, texture_name);

            load_texture(texture_path, current);
        }
    }

    fclose(fp);
}

void draw_model(model *m, context *cont, camera *cam) {
    for (int i = 0; i < m->mesh_count; i++) 
        draw_mesh(m->meshes[i], cont, &m->trans, cam);
}