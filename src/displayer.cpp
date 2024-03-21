#include "displayer.h"

Display::Display() {
    SetConsoleTitle(TEXT(TITLE));
    consoleWriteArea = {0, 0, SCREEN_C-1, SCREEN_R-1};
    windowSize = {0, 0, SCREEN_C-1, SCREEN_R-1};
    bufferSize = {SCREEN_C, SCREEN_R};
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
    SetConsoleScreenBufferSize(wHnd, bufferSize);
}

void Display::Print(std::vector<std::vector<Land>> land_pieces, const std::vector<Being*> CreatureList, const std::vector<Construct*> BuildingList) {
    // Create screen buffer
    std::vector<std::vector<CHAR_INFO>> buffer(SCREEN_R, std::vector<CHAR_INFO>(SCREEN_C));
    try {
        // Add land layer
        for (int i=0; i<SCREEN_R; i++) {
            for (int j=0; j<SCREEN_C; j++) {
                buffer[i][j].Char.AsciiChar = land_pieces[i][j].current.glyph;
                buffer[i][j].Attributes = land_pieces[i][j].current.color;
            }
        }
        // Add building layer
        for (const Construct* el: BuildingList) {
            // cycle through all pts in construct
            for (auto pt: el->PointStructs) {
                buffer[pt.pos.first][pt.pos.second].Char.AsciiChar = pt.glyph;
                buffer[pt.pos.first][pt.pos.second].Attributes = pt.color;
            }
        }
        // Add creature layer
        for (const Being* el : CreatureList) {
            buffer[el->pos.first][el->pos.second].Char.AsciiChar = el->glyph;
            buffer[el->pos.first][el->pos.second].Attributes = el->color;
        }


        // Output screen
        for (int i=0; i<SCREEN_R; i++) {
            for (int j=0; j<SCREEN_C; j++) {
                consoleBuffer[j + SCREEN_C * i].Char.AsciiChar = buffer[i][j].Char.AsciiChar;
                consoleBuffer[j + SCREEN_C * i].Attributes = buffer[i][j].Attributes;
            }
        }
        WriteConsoleOutputA(wHnd, consoleBuffer, endPoints, startPoints, &consoleWriteArea);
    } catch (...) {
        std::cout << "ERROR -- failed to print to display" << std::endl;
    }
}