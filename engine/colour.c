#include "engine/colour.h"
#include "engine/util.h"

#include <math.h>

char palette[]    = " .-:=+!LJf3E6#$@";
int  palette_size = sizeof(palette) - 1;

colour16 RGB_colour16(RGB rgb) {
    // Convert RGB to HSV
    float nr, ng, nb;

    nr = (float)rgb.r / 255.0f;
    ng = (float)rgb.g / 255.0f;
    nb = (float)rgb.b / 255.0f;

    float max = MAX(MAX(nr, ng), nb);
    float min = MIN(MIN(nr, ng), nb);

    float delta = max - min;

    float h = 60.0f;

    if (delta == 0.0f)  { h = 0; } 
    else if (nr == max) { h *= fmodf(((ng - nb) / delta), 6.0f); } 
    else if (ng == max) { h *= ((nb - nr) / delta) + 2.0f; } 
    else                { h *= ((nr - ng) / delta) + 4.0f; } 

    if (h < 0.0f) { h += 360.0f; }

    float s = 0;

    if (max != 0.0f) { s = delta / max; } 

    float br = max;

    if (br <= 0.1f) { return BLACK; }

    // If colour is desaturated(grey)
    // TODO: tweak thresholds
    if (s <= 0.2f) {
        if (br <= 0.5f)      { return BBLACK; }
        else if (br <= 0.9f) { return WHITE;  }
        else                 { return BWHITE; }
    }

    // There are 6 hues RGB and YCM interlaced, equally spaced 60 degrees
    // on the colour wheel: RYGCBM. H is the degrees from R in the direction 
    // of G, e.g. H(Y) = 60, H(G) = 120, etc.

    colour16 out;

    // Multiple of 60
    int n = (int)round(h / 60.0f);

    switch (n) {
    case 0: out = RED;      break;
    case 1: out = YELLOW;   break;
    case 2: out = GREEN;    break;
    case 3: out = CYAN;     break;
    case 4: out = BLUE;     break;
    case 5: out = MAGENTA;  break;
    case 6: out = RED;      break;
    }

    if (s <= 0.5f) {
        out = brighten(out);
    }
    
    return out;
}

colour16 brighten(colour16 c) {
    return c + 60;
}

char RGB_pixel(RGB rgb){
    int brightness = (rgb.r + rgb.g + rgb.b) / 3;
    
    return palette[brightness / (256 / palette_size)];
}