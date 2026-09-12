#include "../include/lua_bindings.h"
#include "../include/inventory.h"
#include "../include/audio.h"
#include "../include/scene.h"
#include <lauxlib.h>

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

void lua_register_bindings(lua_State* L) {
    lua_newtable(L);
    
    lua_pushcfunction(L, l_add_item);     lua_setfield(L, -2, "add_item");
    lua_pushcfunction(L, l_has_item);     lua_setfield(L, -2, "has_item");
    lua_pushcfunction(L, l_audio_play_sfx);   lua_setfield(L, -2, "play_sound");
    lua_pushcfunction(L, l_change_scene); lua_setfield(L, -2, "change_scene");
    lua_pushcfunction(L, l_audio_play_ambient); lua_setfield(L, -2, "play_ambient");
    lua_setglobal(L, "Engine");
}
