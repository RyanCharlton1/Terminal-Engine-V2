#include "engine/mesh.h"
#include "engine/util.h"

triangle transform(mat4 trans, triangle* src) {
    triangle dst;
    
    // Retain uv coords
    dst.left.uv[0]  = src->left.uv[0];
    dst.left.uv[1]  = src->left.uv[1];
    dst.top.uv[0]   = src->top.uv[0];
    dst.top.uv[1]   = src->top.uv[1];
    dst.right.uv[0] = src->right.uv[0];
    dst.right.uv[1] = src->right.uv[1];

    glm_mat4_mulv(trans, src->left.pos,  dst.left.pos);
    glm_mat4_mulv(trans, src->top.pos,   dst.top.pos);
    glm_mat4_mulv(trans, src->right.pos, dst.right.pos);

    return dst;
}

float implicit_line(vec2 a, vec2 b, vec2 p){
    //𝑙𝑖𝑛𝑒(𝐴, 𝐵, 𝑃) = (𝐵𝑦 − 𝐴𝑦)𝑃𝑥 + (𝐴𝑥 − 𝐵𝑥)𝑃𝑦 + 𝐵𝑥𝐴𝑦 − 𝐴𝑥𝐵𝑦
    return (b[1] - a[1])*p[0] + (a[0] - b[0])*p[1] + b[0]*a[1] - a[0]*b[1];
}

void barycentric(
    vec2 p, vec2 left, vec2 top, vec2 right, 
    float* alpha, float *beta, float* gamma) {  

    // Add 0.5 to get the pixel's center
    vec2 p_ = { p[0]+0.5f, p[1]+0.5f };

    *alpha = implicit_line(top, right, p_)  / implicit_line(top, right, left);
    *beta  = implicit_line(left, right, p_) / implicit_line(left, right, top);
    *gamma = implicit_line(left, top, p_)   / implicit_line(left, top, right);
}


int inside_triangle(float alpha, float beta, float gamma) {
    return alpha >= 0 && beta >= 0 && gamma >= 0;
}

void rasterise(triangle* t, context* cont, texture* text) {
    // Divide by w component used in perspective calc
    vec3 div_left, div_top, div_right;

    glm_vec3_divs(t->left.pos,  t->left.pos[3],  div_left);
    glm_vec3_divs(t->top.pos,   t->top.pos[3],   div_top);
    glm_vec3_divs(t->right.pos, t->right.pos[3], div_right);

    // Translate to screenspace pixel indices
    vec2 ss_left, ss_top, ss_right;

    screenspace(cont, div_left,  ss_left);
    screenspace(cont, div_top,   ss_top);
    screenspace(cont, div_right, ss_right);

    // Calculate bounds to draw triangle in
    int maxx = MAX(MAX(ss_left[0], ss_top[0]), ss_right[0]);
    int minx = MIN(MIN(ss_left[0], ss_top[0]), ss_right[0]);
    int maxy = MAX(MAX(ss_left[1], ss_top[1]), ss_right[1]);
    int miny = MIN(MIN(ss_left[1], ss_top[1]), ss_right[1]);

    // Clamp values 
    maxx = clamp(maxx, 0, cont->width - 1);
    minx = clamp(minx, 0, cont->width - 1);
    maxy = clamp(maxy, 0, cont->height - 1);
    miny = clamp(miny, 0, cont->height - 1);

    for (int y = miny; y < maxy; y++) {
        for (int x = minx; x < maxx; x++) {
            // Barycentric coords are the solution to the equation
            // P = a A + b B + g C where ABC is the triangle and a + b + g = 1
            float alpha, beta, gamma;
            vec2 p = { (float)x, (float)y };

            barycentric(p, ss_left, ss_top, ss_right, &alpha, &beta, &gamma);

            if (inside_triangle(alpha, beta, gamma)) {
                // The current pixel is the linear combination of the 
                // triangle's corners multiplied by barycentric coords 
                float depth = alpha * div_left[2]
                            + beta  * div_top[2]
                            + gamma * div_right[2];

                // Perspective projection mat flips the z values so even though
                // negative z direction is forward in the world, after 
                // perspective projection, negative z is behind camera

                // If negative depth(behind camera) discard
                if (depth < 0.0f) { continue; }

                // If not closer than depth buffer value, discard
                if (cont->depth_buffer[y * cont->width + x] < depth)
                    continue;

                // Update depth buffer for this fragment
                cont->depth_buffer[y * cont->width + x] = depth;

                vec2 uv = { 0.0f, 0.0f };
                glm_vec2_muladds(t->left.uv,  alpha, uv);
                glm_vec2_muladds(t->top.uv,   beta,  uv);
                glm_vec2_muladds(t->right.uv, gamma, uv);

                cont->colour_buffer[y * cont-> width + x] = sampleuv(text, uv);
            }
        }
    }
}