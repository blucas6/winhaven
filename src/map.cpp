#include "map.h"
#include "graphics.h"

int ID = 1;

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
}

Land::Land(LandTile _ground, LandTile _plant) {
    ground = _ground;
    ground.init();
    plant = _plant;
    plant.init();
    current = ground;
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

// TYPES ///////////////////////////////////
Dirt::Dirt() : LandTile("Dirt", ID++, {FG_DARKYELLOW}, {'.'}) {

}

Rock::Rock() : LandTile("Rock", ID++, {FG_DARKGREY, FG_LIGHTGREY}, {':', '.'}) {

}

Grass::Grass() : LandTile("Grass", ID++, {FG_GREEN}, {'\'', '"', '`', ',', ';'}) {

}

Moss::Moss() : LandTile("Moss", ID++, {FG_DARKGREEN}, {'*'}) {

}
////////////////////////////////////////////

// MAP /////////////////////////////////////
void MapSlice::generateLand() {
    // TODO: implement an actual noise algorithm
    int type;
    for (int i=0; i<MAP_ROWS; i++) {
        for (int j=0; j<MAP_COLS; j++) {
            type = 1 + rand() % 10;
            if (type < 3) land_array[i][j] = 1;
            else land_array[i][j] = 0; 
        }
    }
}

void MapSlice::cleanConstructArray() {
    // clear the array of all non moveable structures
    for (int i=0; i<MAP_ROWS; i++) {
        for (int j=0; j<MAP_COLS; j++) {
            construct_array[i][j] = 0;
        }
    }
}

void MapSlice::cleanBlockingArray() {
    // clear the array of all non passable objects
    for (int i=0; i<MAP_ROWS; i++) {
        for (int j=0; j<MAP_COLS; j++) {
            blocking_array[i][j] = 0;
        }
    }
}

MapSlice::MapSlice(CConsoleLoggerEx *_debugconsole) : land_array(MAP_ROWS, std::vector<int>(MAP_COLS)), 
land_pieces(MAP_ROWS, std::vector<Land>(MAP_COLS)), blocking_array(MAP_ROWS, std::vector<int>(MAP_COLS)), construct_array(MAP_ROWS, std::vector<int>(MAP_COLS)) {
    DEBUG_CONSOLE = _debugconsole;
}

void MapSlice::mapGen() {
    LandBinder.push_back(new Land(Dirt(), Grass()));       // add all land pieces
    LandBinder.push_back(new Land(Rock(), Moss()));
    generateLand();
    getMapGlyphs();
    cleanBlockingArray();
    cleanConstructArray();
    generateCreatures();
    DEBUG_CONSOLE->cprintf("[map]\tmap gen DONE\n");
}

void MapSlice::generateCreatures() {
    std::pair<int,int> pos;
    Jobs job;
    std::vector<Jobs> jobtypes = {NOJOB, BREWER, FARMER, PRIEST};
    for(int i=0; i<humanAmount; i++) {
        pos = {rand() % MAP_ROWS, rand() % MAP_COLS};
        job = jobtypes[rand() % jobtypes.size()];
        CreatureList.push_back(new Human(pos, job, &BuildingList, DEBUG_CONSOLE, &blocking_array, &construct_array));
    }
}

void MapSlice::getMapGlyphs() {
    // after land values are generated - create the glyphs for the land
    int val = 0;
    for (int i=0; i<MAP_ROWS; i++) {
        for (int j=0; j<MAP_COLS; j++) {
            val = land_array[i][j];
            if (val >= 0 && val < int(LandBinder.size())) {
                LandBinder[val]->current.init();
                // future: land array values will need to be normalized to a certain int value for the binder to work
                land_pieces[i][j] = *LandBinder[val];
                if (rand() % 10 > 1) land_pieces[i][j].growth();
            } else {
                land_pieces[i][j].current.ID = UNKNOWN_ID;
                land_pieces[i][j].current.name = UNKNOWN_NAME;
                land_pieces[i][j].current.glyph = UNKNOWN_GLYPH;
                land_pieces[i][j].current.color = UNKNOWN_COLOR;
            }
        }
    }
}

void MapSlice::updateBlockingArray() {
    // add construction points to blocking array
    blocking_array = construct_array;
    // add creature points to blocking array
    for (int i=0; i<CreatureList.size(); i++) {
        blocking_array[CreatureList[i]->pos.first][CreatureList[i]->pos.second] = 1;
    }
}

void MapSlice::Update() {
    // update creatures
    for (int i=0; i<CreatureList.size(); i++) {
        cleanBlockingArray();
        updateBlockingArray();
        CreatureList[i]->Update();
    }
}
//////////////////////////////////////////////////////
