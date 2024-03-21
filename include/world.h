#ifndef WORLD_H
#define WORLD_H

#include "map.h"
#include "config.h"
#include "enums.h"
#include <vector>
#include "ConsoleLogger.h"

class World {
    public:
        World(CConsoleLoggerEx *_debugconsole);
        CConsoleLoggerEx *DEBUG_CONSOLE;
        MapSlice gMap;
        
        void WorldGen();
        void UpdateMapSlices();
        void UpdateBuildings();
        void UpdateBlockingArray();
};


#endif