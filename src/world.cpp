#include "world.h"

World::World(CConsoleLoggerEx *_debugconsole) : gMap(_debugconsole) {
    DEBUG_CONSOLE = _debugconsole;
}

void World::WorldGen() {
    // TODO: create a creature generation algorithm
    DEBUG_CONSOLE->cprintf("[world]\tStarting world gen...\n");

    gMap.mapGen();
    
    DEBUG_CONSOLE->cprintf("[world]\tWorld gen DONE\n");
}

void World::UpdateMapSlices() {
    gMap.Update();
}

void World::UpdateBuildings() {

}

