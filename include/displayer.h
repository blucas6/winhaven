#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include "config.h"
#include "map.h"
#include "being.h"
#include "construct.h"
#include "graphics.h"

class Display {
    public:
        Display();
        void Display::Print(std::vector<std::vector<Land>> land_pieces, const std::vector<std::shared_ptr<Being>> CreatureList, const std::vector<std::vector<std::shared_ptr<PointStruct>>> PointStruct_Array);
        HANDLE wHnd; /* write (output) handle */
        HANDLE rHnd; /* read (input) handle */
        SMALL_RECT windowSize;
        COORD bufferSize;
        COORD startPoints = {0, 0};
        COORD endPoints = {SCREEN_C, SCREEN_R};
        SMALL_RECT consoleWriteArea;
        CHAR_INFO consoleBuffer[SCREEN_R * SCREEN_C];
        std::pair<int,int> view = {0,0};
        
};


#endif