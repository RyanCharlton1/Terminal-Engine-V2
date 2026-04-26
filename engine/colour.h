#ifndef ENGINE_COLOUR
#define ENGINE_COLOUR

extern char palette[];
extern int  palette_size;

typedef struct _RGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

typedef enum {
    RESET = 0,
    BLACK = 30,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    // Bright colours
    BBLACK = 90,
    BRED,
    BGREEN,
    BYELLOW,
    BBLUE,
    BMAGENTA,
    BCYAN,
    BWHITE,
} colour16;

colour16 RGB_colour16(RGB rgb);
colour16 brighten(colour16 c);

char RGB_pixel(RGB rgb);

#endif