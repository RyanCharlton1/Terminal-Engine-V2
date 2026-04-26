#ifndef ENINGE_INPUT
#define ENINGE_INPUT

#define KEY_COUNT 256

typedef enum {
    RELEASED = 0x00, 
    PRESSED  = 0x01, 
    HELD     = 0x80, 
} key_state;

#endif