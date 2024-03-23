#include "world.h"

World::World() {

}

World::World(CConsoleLoggerEx *_debugconsole) : gMap(_debugconsole) {
    DEBUG_CONSOLE = _debugconsole;
}

void World::WorldGen() {
    // TODO: create a creature generation algorithm
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[world]\tStarting world gen...\n");

    gMap.mapGen();
    
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[world]\tWorld gen DONE\n");
}

void World::UpdateMapSlices() {
    gMap.Update();
}

void World::UpdateBuildings() {

}

