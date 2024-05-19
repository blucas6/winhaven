#include "land.h"

int LAND_ID = 1;

// LANDTILES ///////////////////////////////////////////////////////
LandTile::LandTile() {
    name    = "???";
    ID      = -1;
    colors  = {UNKNOWN_COLOR};
    glyphs  = {UNKNOWN_GLYPH};
}

LandTile::LandTile(std::string _name, int _ID, std::vector<int> _colors, std::vector<char> _glyphs) {
    name = _name;
    ID = _ID;
    colors = _colors;
    glyphs = _glyphs;
    init();
}

// init randomizes the multiple looks a tile could have
void LandTile::init() {         
    color = colors[rand() % colors.size()];
    glyph = glyphs[rand() % glyphs.size()];
}
//////////////////////////////////////////////////////////////

// LAND //////////////////////////////////////////////////////
Land::Land() {
    LandTile default_tile;
    ground = default_tile;
    plant = default_tile;
    current = default_tile;
    tilled = default_tile;
}

Land::Land(LandTile _ground, LandTile _plant, LandTile _tilled) {
    ground = _ground;
    ground.init();
    plant = _plant;
    plant.init();
    current = ground;
    tilled = _tilled;
    tilled.init();
}

void Land::growth() {
    // every type of land has the possibility for growing a plant
    current.name = plant.name;
    current.ID = plant.ID;
    current.colors = plant.colors;
    current.glyphs = plant.glyphs;
    current.init();
}

void Land::ungrowth() {
    current.name = ground.name;
    current.ID = ground.ID;
    current.colors = ground.colors;
    current.glyphs = ground.glyphs;
    current.init();
}

void Land::till() {
    current.name = tilled.name;
    current.ID = tilled.ID;
    current.colors = tilled.colors;
    current.glyphs = tilled.glyphs;
    current.init();
}

// TYPES ///////////////////////////////////
Dirt::Dirt() : LandTile("Dirt", LAND_ID++, {FG_DARKYELLOW}, {'.'}) {}

Rock::Rock() : LandTile("Rock", LAND_ID++, {FG_DARKGREY, FG_LIGHTGREY}, {':', '.'}) {}

Grass::Grass() : LandTile("Grass", LAND_ID++, {FG_GREEN}, {'\'', '"', '`', ',', ';'}) {}

Moss::Moss() : LandTile("Moss", LAND_ID++, {FG_DARKGREEN}, {'*'}) {}

Tilled::Tilled() : LandTile("Tilled land", LAND_ID++, {FG_DARKYELLOW}, {'~'}) {}

Barley::Barley() : LandTile("Barley", LAND_ID++, {FG_DARKYELLOW}, {char(157)}) {}

Hay::Hay() : LandTile("Hay", LAND_ID++, {FG_YELLOW}, {'"'}) {}

Blueberry::Blueberry() : LandTile("Blueberry", LAND_ID++, {FG_DARKBLUE}, {'.'}) {}

////////////////////////////////////////////
