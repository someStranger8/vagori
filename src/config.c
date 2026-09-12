#include "../include/config.h"
#include "../lib/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool config_load(const char* json_path, GameConfig* config) {
    strncpy(config->title, "Point & Click Game", 127);
    config->window_width = 800;
    config->window_height = 600;
    config->fullscreen = false;
    strncpy(config->default_scene, "game/scenes/default.json", 255);
    strncpy(config->main_script, "game/scripts/main.lua", 255);

    FILE* file = fopen(json_path, "rb");
    if (!file) {
        printf("[WARN] Could not find %s, using defaults.\n", json_path);
        return false;
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
    if (!json) return false;

    cJSON* title = cJSON_GetObjectItemCaseSensitive(json, "title");
    if (cJSON_IsString(title)) strncpy(config->title, title->valuestring, 127);

    cJSON* window = cJSON_GetObjectItemCaseSensitive(json, "window");
    if (cJSON_IsObject(window)) {
        cJSON* w = cJSON_GetObjectItemCaseSensitive(window, "width");
        cJSON* h = cJSON_GetObjectItemCaseSensitive(window, "height");
        cJSON* fs = cJSON_GetObjectItemCaseSensitive(window, "fullscreen");

        if (cJSON_IsNumber(w)) config->window_width = w->valueint;
        if (cJSON_IsNumber(h)) config->window_height = h->valueint;
        if (cJSON_IsBool(fs)) config->fullscreen = cJSON_IsTrue(fs);
    }

    cJSON* entry = cJSON_GetObjectItemCaseSensitive(json, "entry");
    if (cJSON_IsObject(entry)) {
        cJSON* scene = cJSON_GetObjectItemCaseSensitive(entry, "default_scene");
        cJSON* script = cJSON_GetObjectItemCaseSensitive(entry, "main_script");

        if (cJSON_IsString(scene)) strncpy(config->default_scene, scene->valuestring, 255);
        if (cJSON_IsString(script)) strncpy(config->main_script, script->valuestring, 255);
    }

    cJSON_Delete(json);
    return true;
}
