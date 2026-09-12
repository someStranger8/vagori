#ifndef INVENTORY_H
#define INVENTORY_H

#include <SDL2/SDL.h>
#include <stdbool.h>

bool inventory_has(const char* item_name);
void inventory_add(const char* item_name);

#endif
