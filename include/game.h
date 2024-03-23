#ifndef GAME_H
#define GAME_H

#include "world.h"
#include "being.h"
#include "displayer.h"
#include "ConsoleLogger.h"

class Game {
    public:
        Game();
        Game(CConsoleLoggerEx *_debugconsole);
        CConsoleLoggerEx *DEBUG_CONSOLE = nullptr;
        World world;
        Display gDisplay;
        void PrintScreen();
        void UpdateWorld();
};

#endif