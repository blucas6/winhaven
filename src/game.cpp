#include "game.h"

Game::Game() {
    world.WorldGen();
}

Game::Game(CConsoleLoggerEx *_debugconsole) : world(_debugconsole) {
    DEBUG_CONSOLE = _debugconsole;
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[game]\tStarting game...\n");
    world.WorldGen();
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("\n[game]\tGame is READY\n");
}

void Game::UpdateWorld() {
    world.UpdateMapSlices();
}

void Game::PrintScreen() {
    std::vector<Being*> list;
    list.insert(list.end(), world.gMap.CreatureList.begin(), world.gMap.CreatureList.end());
    // add creatures from towns to print out
    for(int t=0; t<world.gMap.TownList.size(); t++) {
        list.insert(list.end(), world.gMap.TownList[t].CreatureList.begin(), world.gMap.TownList[t].CreatureList.end());
    }
    gDisplay.Print(world.gMap.land_pieces, list, world.gMap.BuildingList);
}