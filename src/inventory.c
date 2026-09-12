#include "../include/inventory.h"
#include "../include/engine.h"
#include <string.h>

bool inventory_has(const char* item_name) {
    for (int i = 0; i < g_engine.inventory_count; ++i) {
        if (strcmp(g_engine.inventory[i], item_name) == 0) return true;
    }
    return false;
}

void inventory_add(const char* item_name) {
    if (g_engine.inventory_count < MAX_ITEMS && !inventory_has(item_name)) {
        strncpy(g_engine.inventory[g_engine.inventory_count], item_name, 63);
        g_engine.inventory_count++;
    }
}

void inventory_render(SDL_Renderer* renderer) {
    SDL_Rect bar = { 0, 540, 800, INVENTORY_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &bar);

    for (int i = 0; i < g_engine.inventory_count; ++i) {
        SDL_Rect slot = { 10 + (i * 50), 545, 40, 40 };
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        SDL_RenderDrawRect(renderer, &slot);
    }
}
