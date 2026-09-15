#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "../lib/miniaudio.h"
#include "config.h"
#include <stdbool.h>

#define MAX_HOTSPOTS 16
#define MAX_ITEMS 32
#define INVENTORY_HEIGHT 60

typedef struct {
    char name[64];
    SDL_Rect rect;
    char lua_callback[64];
} Hotspot;

typedef struct {
    GameConfig config;
    SDL_Window* window;
    SDL_Renderer* renderer;
    lua_State* L;
    ma_engine audio;

    SDL_Texture* bg_texture;
    Hotspot hotspots[MAX_HOTSPOTS];
    int hotspot_count;
    ma_sound ambient_sound;
    bool playing_ambient;

    char inventory[MAX_ITEMS][64];
    int inventory_count;

    bool is_running;
} GameEngine;

extern GameEngine g_engine;

bool engine_init(const char* config_path);
void engine_run(void);
void engine_cleanup(void);

#endif
