#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

#include "construct.h"
#include "ConsoleLogger.h"
#include "config.h"
#include "being.h"

// basic form of what can be displayed
class LandTile {
    public:
        LandTile();
        LandTile(std::string _name, int _ID, std::vector<int> _colors, std::vector<char> _glyphs);
        std::string name;
        int ID;
        int color;
        char glyph;
        std::vector<int> colors;
        std::vector<char> glyphs;
        void init();
};

class Land {
    public:
        Land(); // need default for map array
        Land(LandTile ground, LandTile plant);
        LandTile current;
        LandTile ground;
        LandTile plant;
        void growth();
        void ungrowth();
};

class Dirt : public LandTile {
    public:
        Dirt();
};

class Rock : public LandTile {
    public:
        Rock();
};

class Grass : public LandTile {
    public:
        Grass();
};

class Moss : public LandTile {
    public:
        Moss();
};

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

        std::vector<Being*> CreatureList;
        std::vector<Construct*> BuildingList;

        int humanAmount = 4;

        void mapGen();
        void generateLand();
        void getMapGlyphs();
        void cleanBlockingArray();
        void cleanConstructArray();
        void updateBlockingArray();
        void generateCreatures();
        void Update();
};

#endif