#include "engine/engine.h"
#include "engine/context.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <dirent.h>
#include <fcntl.h>

int find_keyboard_fd() {
    DIR* dir = opendir("/dev/input");

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, "event", 5) != 0) { continue; }
        
        char path[512];
        sprintf(path, "/dev/input/%s", entry->d_name);
        
        int fd = open(path, O_RDONLY | O_NONBLOCK);
        if (fd < 0) { continue; }
        
        uint8_t evbits[EV_MAX / 8 + 1] = {0};
        ioctl(fd, EVIOCGBIT(0, sizeof(evbits)), evbits);
        
        // Check if device has key events
        if (evbits[EV_KEY / 8] & (1 << (EV_KEY % 8))) {
            // Check if it has enough keys to be a keyboard (has KEY_SPACE)
            uint8_t keybits[KEY_MAX / 8 + 1] = {0};
            ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybits)), keybits);
            if (keybits[KEY_SPACE / 8] & (1 << (KEY_SPACE % 8))) {
                closedir(dir);
                return fd; // found a keyboard
            }
        }
        close(fd);
    }

    fprintf(stderr, "failed to find keyboard\n");
    closedir(dir);
    return -1;
}

#endif

// malloc engine and init key structs 
engine* init_engine(int width, int height, colour_depth depth) {
    engine* e = (engine*)malloc(sizeof(engine));

    if (!e) { printf("malloc failed: init_engine\n"); return NULL; }

    memset(e, 0, sizeof(engine));

    e->current_context = init_context(width, height, depth); 
    e->current_camera  = init_camera();

    return e;
}

void free_engine(engine* e) {
    if (e->current_context) { free_context(e->current_context); }
    if (e->current_camera)  { free_camera(e->current_context); }
    free(e);
}

void start_engine(engine* e) {
    e->init_resources(e);

    config_terminal(e);

#ifdef __linux__
    keyboard_fd = find_keyboard_fd();
#endif

    e->running = 1;

    double  frame_time  = 0.0;
    clock_t frame_start = clock();
    while (e->running) {
        poll_input(e);

        wipe_buffers(e->current_context);

        e->logic(frame_time, e);

        e->render(e);

        print_context(e->current_context);

        time_t frame_end = clock();

        frame_time = (double)(frame_end - frame_start) / CLOCKS_PER_SEC;

        frame_start = frame_end;
    }

    e->free_resources(e);

    restore_terminal(e);

#ifdef __linux__
    close(keyboard_fd);
#endif
}

// Linux doesn't support held for now because API uses events not polling 
// states. Windows API reports keys as ASCII, Linux does numerically from 
// top left of standard US/UK keyboard to bottom right e.g. KEY_ESC == 1
void poll_input(engine* self) {
#ifdef _WIN32
    for (int i = 0; i < KEY_COUNT; i++) {
        if (GetKeyState(i) & 0x8000) { 
            if (e->keys[i]) { e->keys[i] = HELD; } 
            else            { e->keys[i] = PRESSED; }
        } else { e->keys[i] = RELEASED; }
    }
#else

    struct input_event input;
    while (read(keyboard_fd, &input, sizeof(input)) > 0) {
        if (input.type == EV_KEY) {
            // Linux keycodes aren't in alphabetical order...
            // Only supports a-z keys for now
            key_state new_state = RELEASED;

            // Non-zero value encompasses both pressed and held updates
            if (input.value) { new_state = HELD; }

            switch (input.code) {
            case KEY_Q: self->keys['Q'] = new_state; break;
            case KEY_W: self->keys['W'] = new_state; break;
            case KEY_E: self->keys['E'] = new_state; break;
            case KEY_R: self->keys['R'] = new_state; break;
            case KEY_T: self->keys['T'] = new_state; break;
            case KEY_Y: self->keys['Y'] = new_state; break;
            case KEY_U: self->keys['U'] = new_state; break;
            case KEY_I: self->keys['I'] = new_state; break;
            case KEY_O: self->keys['O'] = new_state; break;
            case KEY_P: self->keys['P'] = new_state; break;
            case KEY_A: self->keys['A'] = new_state; break;
            case KEY_S: self->keys['S'] = new_state; break;
            case KEY_D: self->keys['D'] = new_state; break;
            case KEY_F: self->keys['F'] = new_state; break;
            case KEY_G: self->keys['G'] = new_state; break;
            case KEY_H: self->keys['H'] = new_state; break;
            case KEY_J: self->keys['J'] = new_state; break;
            case KEY_K: self->keys['K'] = new_state; break;
            case KEY_L: self->keys['L'] = new_state; break;
            case KEY_Z: self->keys['Z'] = new_state; break;
            case KEY_X: self->keys['X'] = new_state; break;
            case KEY_C: self->keys['C'] = new_state; break;
            case KEY_V: self->keys['V'] = new_state; break;
            case KEY_B: self->keys['B'] = new_state; break;
            case KEY_N: self->keys['N'] = new_state; break;
            case KEY_M: self->keys['M'] = new_state; break;                             
            }
        }
    }
    
#endif
}

void print_target_os() {

#ifdef _WIN32
    const char* os_str = "windows";
#else 
    const char* os_str = "linux";
#endif

    printf("os: %s\n", os_str);
}

void config_terminal(engine* self) {
    int width  = self->current_context->width;
    int height = self->current_context->height;

    printf("\033[8;%d;%dt", height, width); // Resize terminal

    printf(DISABLE_CURSOR_ESCAPE);

#ifdef _WIN32
#else
    tcgetattr(STDIN_FILENO, &original_terminal);
    
    altered_terminal = original_terminal;
    altered_terminal.c_lflag ^= ECHO;

    tcsetattr(STDOUT_FILENO, TCSANOW, &altered_terminal);
#endif
}

void restore_terminal(engine* self) {

    printf(ENABLE_CURSOR_ESCAPE);
    printf(RESET_COLOUR_ESCAPE); // Reset terminal colour

#ifdef _WIN32
#else
    // Flush to stop input going to terminal on exit
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal);
#endif
}