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

void Display::Print(std::vector<std::vector<Land>> land_pieces, const std::vector<Being*> CreatureList, const std::vector<std::vector<std::shared_ptr<PointStruct>>> PointStruct_Array) {
    // Create screen buffer
    std::vector<std::vector<CHAR_INFO>> buffer(SCREEN_R, std::vector<CHAR_INFO>(SCREEN_C));
    try {
        // Add land layer
        for (int i=0; i<SCREEN_R; i++) {
            for (int j=0; j<SCREEN_C; j++) {
                if (view.first+i<0 || view.first+i>=MAP_ROWS || view.second+j<0 || view.second+j>=MAP_COLS) {
                    buffer[i][j].Char.AsciiChar = 0;
                    buffer[i][j].Attributes = FG_BLACK | BG_BLACK;
                } else {
                    buffer[i][j].Char.AsciiChar = land_pieces[view.first+i][view.second+j].current.glyph;
                    buffer[i][j].Attributes = land_pieces[view.first+i][view.second+j].current.color;
                }
            }
        }
        // Add building layer
        std::pair<int,int> realpos;
        for (int i=0; i<MAP_ROWS; i++) {
            for (int j=0; j<MAP_COLS; j++) {
                if (PointStruct_Array[i][j] != nullptr) {
                    realpos.first = PointStruct_Array[i][j]->pos.first - view.first;
                    realpos.second = PointStruct_Array[i][j]->pos.second - view.second;
                    if (realpos.first>=0 && realpos.first<SCREEN_R && realpos.second>=0 && realpos.second<SCREEN_C) {
                        buffer[realpos.first][realpos.second].Char.AsciiChar = PointStruct_Array[i][j]->glyph;
                        buffer[realpos.first][realpos.second].Attributes = PointStruct_Array[i][j]->color;
                    }
                }
            }
        }
        // Add creature layer
        for (const Being* el : CreatureList) {
            realpos.first = el->pos.first - view.first;
            realpos.second = el->pos.second - view.second;
            if (realpos.first>=0 && realpos.first<SCREEN_R && realpos.second>=0 && realpos.second<SCREEN_C) {
                buffer[realpos.first][realpos.second].Char.AsciiChar = el->glyph;
                buffer[realpos.first][realpos.second].Attributes = el->color;
            }
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