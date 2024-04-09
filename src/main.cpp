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
    srand((unsigned) time(NULL));
	DEBUG_CONSOLE.cls(CConsoleLoggerEx::COLOR_BACKGROUND_BLACK);
	DEBUG_CONSOLE.gotoxy(0,0);
	DEBUG_CONSOLE.cprintf( CConsoleLoggerEx::COLOR_WHITE| CConsoleLoggerEx::COLOR_BACKGROUND_BLACK,"[main]\tDebugger is ON\n");

    Game game(&DEBUG_CONSOLE);
    game.PrintScreen();
    char cmd;
    // MAIN LOOP
    while(true) {
        cmd = _getch();
        if (cmd == 'q') {
            // game.EndGame();
            break; 
        }
        else if (cmd == 'a') game.gDisplay.view.second--;
        else if (cmd == 'w') game.gDisplay.view.first--;
        else if (cmd == 's') game.gDisplay.view.first++;
        else if (cmd == 'd') game.gDisplay.view.second++;
        else if (cmd == 'p') game.printPointStructArray();
        else if (cmd == 'g') game.moveBeings();
        else if (cmd == 'b') game.printBlockingArray();
        else {
            game.UpdateWorld();
        }
        game.PrintScreen();
    }

    return 0;
}
