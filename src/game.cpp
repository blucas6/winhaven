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
    std::vector<std::shared_ptr<Being>> being_list;
    being_list.insert(being_list.end(), world.gMap.CreatureList.begin(), world.gMap.CreatureList.end());
    // add creatures from towns to print out
    for(int t=0; t<world.gMap.TownList.size(); t++) {
        being_list.insert(being_list.end(), world.gMap.TownList[t].CreatureList.begin(), world.gMap.TownList[t].CreatureList.end());
    }
    gDisplay.Print(world.gMap.land_pieces, being_list, world.gMap.PointStructs_Array);

    being_list.clear();
}

void Game::moveBeings() {
    for (std::shared_ptr<Being> b : world.gMap.TownList[0].CreatureList) {
        b->goToGOTOPT();
    }
}

void Game::printPointStructArray() {
    if (DEBUG_CONSOLE != nullptr) {
        DEBUG_CONSOLE->cprintf("[game]\tPointStruct array:\n");
        for (int i=0; i<MAP_ROWS; i++) {
            for (int j=0; j<MAP_COLS; j++) {
                if (world.gMap.PointStructs_Array[i][j] == nullptr) DEBUG_CONSOLE->cprintf(".");
                else DEBUG_CONSOLE->cprintf("%x", world.gMap.PointStructs_Array[i][j]->type);
            }
        }
    }
}

void Game::printBlockingArray() {
    if (DEBUG_CONSOLE != nullptr) {
        DEBUG_CONSOLE->cprintf("[game]\tBlocking array:\n");
        for (int i=0; i<MAP_ROWS; i++) {
            for (int j=0; j<MAP_COLS; j++) {
                DEBUG_CONSOLE->cprintf("%d", world.gMap.blocking_array[i][j]);
            }
        }
    }
}

void Game::EndGame() {
    
}