#include "game.h"

Game::Game(CConsoleLoggerEx *_debugconsole) : world(_debugconsole) {
    DEBUG_CONSOLE = _debugconsole;
    DEBUG_CONSOLE->cprintf("[game]\tStarting game...\n");
    world.WorldGen();
    DEBUG_CONSOLE->cprintf("\n[game]\tGame is READY\n");
}

void Game::UpdateWorld() {
    world.UpdateMapSlices();
}

void Game::PrintScreen() {
    gDisplay.Print(world.gMap.land_pieces, world.gMap.CreatureList, world.gMap.BuildingList);
}