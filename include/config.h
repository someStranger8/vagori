#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

typedef struct {
    char title[128];
    int window_width;
    int window_height;
    bool fullscreen;
    char default_scene[256];
    char main_script[256];
} GameConfig;

bool config_load(const char* json_path, GameConfig* config);

#endif
