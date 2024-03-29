#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

#include "construct.h"
#include "ConsoleLogger.h"
#include "config.h"
#include "being.h"
#include "town.h"
#include "land.h"

class MapSlice {
    public:
        MapSlice();
        MapSlice(CConsoleLoggerEx *_debugconsole);
        CConsoleLoggerEx *DEBUG_CONSOLE = nullptr;

        std::vector<std::vector<int>> land_array;       // array for storage of numerical land values
        std::vector<std::vector<Land>> land_pieces;     // actual char glyphs for displaying
        std::vector<std::vector<int>> blocking_array;   // for pathfinding
        std::vector<std::vector<int>> construct_array;  // save unmoveable objects in here for faster pathfinding
        std::vector<Land*> LandBinder;                  // binder of all possible land pieces

        std::vector<Being*> CreatureList;               // all creatures on landslice
        std::vector<Construct*> BuildingList;           // all buildings on landslice
        std::vector<Town> TownList;                     // array of all towns

        void mapGen();
        void generateLand();
        void getMapGlyphs();
        void cleanBlockingArray();
        void cleanConstructArray();
        void updateBlockingArray();
        void generateCreatures();
        void generateTowns();
        void Update();
};

#endif