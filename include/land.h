#ifndef LAND_H
#define LAND_H

#include <string>
#include <vector>
#include "graphics.h"

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

// Actual land block
//  Holds LandTiles as possible versions of the same land block
class Land {
    public:
        Land(); // need default for map array
        Land(LandTile ground, LandTile plant, LandTile tilled);
        virtual ~Land() {};
        LandTile current;
        LandTile ground;
        LandTile plant;
        LandTile tilled;
        void growth();
        void ungrowth();
        void till();
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

class Tilled : public LandTile {
    public:
        Tilled();
};

class Barley : public LandTile {
    public:
        Barley();
};

class Hay : public LandTile {
    public:
        Hay();
};

class Blueberry : public LandTile {
    public:
        Blueberry();
};

#endif