#include <iostream>
#include <cstdlib>
#include <vector>
#include <conio.h>
#include <thread>

#include "game.h"
#include "ConsoleLogger.h"

int main() {
    // SETUP
    CConsoleLoggerEx DEBUG_CONSOLE;
	DEBUG_CONSOLE.Create("Debugger");
	
	DEBUG_CONSOLE.cls(CConsoleLoggerEx::COLOR_BACKGROUND_BLACK);
	DEBUG_CONSOLE.gotoxy(0,0);
	DEBUG_CONSOLE.cprintf( CConsoleLoggerEx::COLOR_WHITE| CConsoleLoggerEx::COLOR_BACKGROUND_BLACK,"[main]\tDebugger is ON\n");

    srand((unsigned) time(NULL));
    Game game(&DEBUG_CONSOLE);
    game.PrintScreen();
    char cmd;
    // MAIN LOOP
    while(true) {
        game.UpdateWorld();
        game.PrintScreen();
        cmd = _getch();
        if (cmd == 'q') break;
    }

    return 0;
}
