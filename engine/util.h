#ifndef ENGINE_UTIL
#define ENGINE_UTIL

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int clamp(int n, int min, int max);

#endif