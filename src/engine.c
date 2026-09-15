#define MINIAUDIO_IMPLEMENTATION
#define MA_ENABLE_MP3
#include "../lib/miniaudio.h"

#include "../include/engine.h"
#include "../include/config.h"
#include "../include/audio.h"
#include "../include/inventory.h"
#include "../include/scene.h"
#include "../include/lua_bindings.h"
#include <stdio.h>

GameEngine g_engine;

// init engine
bool engine_init(const char* config_path) {
    config_load(config_path, &g_engine.config);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) return false;
    if (ma_engine_init(NULL, &g_engine.audio) != MA_SUCCESS) return false;

    Uint32 flags = g_engine.config.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
    g_engine.window = SDL_CreateWindow(
        g_engine.config.title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        g_engine.config.window_width,
        g_engine.config.window_height,
        flags
    );

    g_engine.renderer = SDL_CreateRenderer(g_engine.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderSetLogicalSize(g_engine.renderer, 427, 240);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    if (!g_engine.window || !g_engine.renderer) return false;

    g_engine.L = luaL_newstate();
    luaL_openlibs(g_engine.L);
    lua_register_bindings(g_engine.L);

    if (luaL_dofile(g_engine.L, g_engine.config.main_script) != LUA_OK) {
        printf("[LUA ERROR] %s\n", lua_tostring(g_engine.L, -1));
    }

    scene_load(g_engine.config.default_scene);
    g_engine.is_running = true;
    return true;
}

// game loop
void engine_run(void) {
    Uint32 last_time = SDL_GetTicks();
    while (g_engine.is_running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) g_engine.is_running = false;

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int mx = e.button.x;
                int my = e.button.y;

                for (int i = 0; i < g_engine.hotspot_count; ++i) {
                    Hotspot* h = &g_engine.hotspots[i];
                    if (mx >= h->rect.x && mx <= h->rect.x + h->rect.w &&
                        my >= h->rect.y && my <= h->rect.y + h->rect.h) {
                        
                        lua_getglobal(g_engine.L, h->lua_callback);
                        if (lua_pcall(g_engine.L, 0, 0, 0) != LUA_OK) {
                            printf("[LUA ERROR] %s\n", lua_tostring(g_engine.L, -1));
                            lua_pop(g_engine.L, 1);
                        }
                        break;
                    }
                }
            }
        }

        SDL_RenderClear(g_engine.renderer);
        if (g_engine.bg_texture) SDL_RenderCopy(g_engine.renderer, g_engine.bg_texture, NULL, NULL);
        SDL_RenderPresent(g_engine.renderer);

        if (g_engine.bg_texture) {
            SDL_RenderCopy(g_engine.renderer, g_engine.bg_texture, NULL, NULL);
        }

        for (int i = 0; i < g_engine.hotspot_count; ++i) {
            Hotspot* h = &g_engine.hotspots[i];
            if (h->is_visible && h->sprite_texture) {
                SDL_RenderCopy(g_engine.renderer, h->sprite_texture, NULL, &h->sprite_rect);
            }
        }

        Uint32 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;
        audio_update(delta_time);

        SDL_Delay(16);
    }
}

void engine_cleanup(void) {
    if (g_engine.playing_ambient) ma_sound_uninit(&g_engine.ambient_sound);
    ma_engine_uninit(&g_engine.audio);
    lua_close(g_engine.L);
    SDL_DestroyRenderer(g_engine.renderer);
    SDL_DestroyWindow(g_engine.window);
    IMG_Quit();
    SDL_Quit();
}
