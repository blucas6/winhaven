#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include <Windows.h>
#include <vector>
#include "map.h"
#include "being.h"
#include "construct.h"
#include "graphics.h"
#include <iostream>

class Display {
    public:
        Display();
        void Print(std::vector<std::vector<Land>> land_pieces, const std::vector<Being*> CreatureList, const std::vector<Construct*> BuildingList);
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