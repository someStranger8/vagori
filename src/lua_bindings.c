#include "../include/lua_bindings.h"
#include "../include/engine.h"
#include "../include/inventory.h"
#include "../include/audio.h"
#include "../include/scene.h"
#include <lauxlib.h>

// wrapper functions for lua bindings
static int l_add_item(lua_State* L) {
    const char* item = luaL_checkstring(L, 1);
    inventory_add(item);
    return 0;
}

static int l_has_item(lua_State* L) {
    const char* item = luaL_checkstring(L, 1);
    lua_pushboolean(L, inventory_has(item));
    return 1;
}

static int l_change_scene(lua_State* L) {
    const char* path = luaL_checkstring(L, 1);
    scene_load(path);
    return 0;
}

static int l_audio_play_ambient(lua_State* L) {
    const char* path = luaL_checkstring(L, 1);
    float fade_time = (float)luaL_optnumber(L, 2, 3.0);
    audio_play_ambient(path, fade_time);
    return 0;
}

static int l_audio_play_sfx(lua_State* L) {
    const char* path = luaL_checkstring(L, 1);
    audio_play_sfx(path);
    return 0;
}

static int l_scene_set_hotspot_visible(lua_State* L) {
    const char* name = luaL_checkstring(L, 1);
    bool visible = lua_toboolean(L, 2);

    for (int i = 0; i < g_engine.hotspot_count; ++i) {
        if (strcmp(g_engine.hotspots[i].name, name) == 0) {
            g_engine.hotspots[i].is_visible = visible;
            break;
        }
    }
    return 0;
}

// expose functions
void lua_register_bindings(lua_State* L) {
    lua_newtable(L);
    
    lua_pushcfunction(L, l_add_item);     lua_setfield(L, -2, "add");
    lua_pushcfunction(L, l_has_item);     lua_setfield(L, -2, "has");
    lua_setglobal(L, "Inventory");
    lua_pushcfunction(L, l_audio_play_sfx);   lua_setfield(L, -2, "play_sound");
    lua_pushcfunction(L, l_audio_play_ambient); lua_setfield(L, -2, "play_ambient");
    lua_setglobal(L, "Audio");
    lua_pushcfunction(L, l_change_scene); lua_setfield(L, -2, "change");
    lua_pushcfunction(L, l_scene_set_hotspot_visible); lua_setfield(L, -2, "hotspot_visible");
    lua_setglobal(L, "Scene");
}
