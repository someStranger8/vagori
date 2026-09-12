#include "../include/engine.h"

int main(int argc, char* argv[]) {
    // can change game folder via cli; similar to quake
    const char* config_file = (argc > 1) ? argv[1] : "game/gameinfo.json";

    if (engine_init(config_file)) {
        engine_run();
    }
    
    engine_cleanup();
    return 0;
}
