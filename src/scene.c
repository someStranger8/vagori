#include "../include/scene.h"
#include "../include/engine.h"
#include "../include/audio.h"
#include "../lib/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void scene_load(const char* json_path) {
    FILE* file = fopen(json_path, "rb");
    if (!file) {
        printf("[ERROR] Failed to open scene: %s\n", json_path);
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(length + 1);
    fread(buffer, 1, length, file);
    fclose(file);
    buffer[length] = '\0';

    cJSON* json = cJSON_Parse(buffer);
    free(buffer);
    if (!json) return;

    if (g_engine.bg_texture) SDL_DestroyTexture(g_engine.bg_texture);
    g_engine.hotspot_count = 0;

    cJSON* bg = cJSON_GetObjectItemCaseSensitive(json, "background");
    if (cJSON_IsString(bg)) {
        SDL_Surface* surface = IMG_Load(bg->valuestring);
        if (!surface) {
            printf("[IMG ERROR] %s\n", IMG_GetError());
        }
        else {
            g_engine.bg_texture = SDL_CreateTextureFromSurface(g_engine.renderer, surface);
            SDL_FreeSurface(surface);
        }
    }

    cJSON* audio = cJSON_GetObjectItemCaseSensitive(json, "audio");
    if (cJSON_IsObject(audio)) {

        cJSON* music = cJSON_GetObjectItemCaseSensitive(audio, "music");
        cJSON* crossfade = cJSON_GetObjectItemCaseSensitive(audio, "crossfade");
        if (cJSON_IsString(music) && cJSON_IsNumber(crossfade)) {
            float fade_time = cJSON_IsNumber(crossfade) ? (float)crossfade->valuedouble : 3.0f;
            audio_play_ambient(music->valuestring, fade_time);
        }
    }

    cJSON* hotspots = cJSON_GetObjectItemCaseSensitive(json, "hotspots");
    cJSON* hp = NULL;
    cJSON_ArrayForEach(hp, hotspots) {
        if (g_engine.hotspot_count >= MAX_HOTSPOTS) break;
        Hotspot* h = &g_engine.hotspots[g_engine.hotspot_count++];
        
        cJSON* name = cJSON_GetObjectItemCaseSensitive(hp, "name");
        cJSON* callback = cJSON_GetObjectItemCaseSensitive(hp, "on_click");
        cJSON* rect = cJSON_GetObjectItemCaseSensitive(hp, "rect");

        if (cJSON_IsString(name)) strncpy(h->name, name->valuestring, 63);
        if (cJSON_IsString(callback)) strncpy(h->lua_callback, callback->valuestring, 63);

        if (cJSON_IsObject(rect)) {
            h->rect.x = cJSON_GetObjectItemCaseSensitive(rect, "x")->valueint;
            h->rect.y = cJSON_GetObjectItemCaseSensitive(rect, "y")->valueint;
            h->rect.w = cJSON_GetObjectItemCaseSensitive(rect, "w")->valueint;
            h->rect.h = cJSON_GetObjectItemCaseSensitive(rect, "h")->valueint;
        }
    }

    cJSON_Delete(json);
}
