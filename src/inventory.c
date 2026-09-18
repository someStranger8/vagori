#include "../include/inventory.h"
#include "../include/engine.h"
#include <string.h>

// check inventory item
bool inventory_has(const char* item_name) {
    for (int i = 0; i < g_engine.inventory_count; ++i) {
        if (strcmp(g_engine.inventory[i], item_name) == 0) return true;
    }
    return false;
}

// add to inventory
void inventory_add(const char* item_name) {
    if (g_engine.inventory_count < MAX_ITEMS && !inventory_has(item_name)) {
        strncpy(g_engine.inventory[g_engine.inventory_count], item_name, 63);
        g_engine.inventory_count++;
    }
}

