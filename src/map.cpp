#include "map.h"

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

void MapSlice::cleanPTArray() {
    // clear the array of all non passable objects
    for (int i=0; i<MAP_ROWS; i++) {
        for (int j=0; j<MAP_COLS; j++) {
            PointStructs_Array[i][j] = new PointStruct();
        }
    }
}

MapSlice::MapSlice() : land_array(MAP_ROWS, std::vector<int>(MAP_COLS)), 
land_pieces(MAP_ROWS, std::vector<Land>(MAP_COLS)), blocking_array(MAP_ROWS, std::vector<int>(MAP_COLS)), construct_array(MAP_ROWS, std::vector<int>(MAP_COLS)) {

}

MapSlice::MapSlice(CConsoleLoggerEx *_debugconsole) : land_array(MAP_ROWS, std::vector<int>(MAP_COLS)), 
land_pieces(MAP_ROWS, std::vector<Land>(MAP_COLS)), blocking_array(MAP_ROWS, std::vector<int>(MAP_COLS)), construct_array(MAP_ROWS, std::vector<int>(MAP_COLS)),
PointStructs_Array(MAP_ROWS, std::vector<PointStruct*>(MAP_COLS)) {
    DEBUG_CONSOLE = _debugconsole;
}

void MapSlice::generateTowns() {
    TownList.push_back(Town());
}

void MapSlice::mapGen() {
    LandBinder.push_back(new Land(Dirt(), Grass(), Tilled()));       // add all land pieces
    LandBinder.push_back(new Land(Rock(), Moss(), Tilled()));
    generateLand();
    getMapGlyphs();
    cleanBlockingArray();
    cleanConstructArray();
    cleanPTArray();
    generateTowns();
    generateCreatures();
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[map]\tmap gen DONE\n");
}

void MapSlice::generateCreatures() {
    if (TownList.size() > 0) {
        DEBUG_CONSOLE->cprintf("[map]\tStarting creature generation\n");
        std::pair<int,int> pos;
        Jobs job;
        std::vector<Jobs> jobtypes = {NOJOB, BREWER, FARMER, PRIEST};
        for(int i=0; i<HUMAN_GEN_AMOUNT; i++) {
            pos = {rand() % MAP_ROWS, rand() % MAP_COLS};
            // job = jobtypes[rand() % jobtypes.size()];
            job = FARMER;
            Human *human = new Human(pos, job, &BuildingList, &land_pieces, DEBUG_CONSOLE, &blocking_array, &construct_array, &PointStructs_Array);
            human->myTown = &TownList[0];
            TownList[0].CreatureList.push_back(human);
        }
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
                if (1+rand() % 10 > 2) land_pieces[i][j].growth();
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
    // if (DEBUG_CONSOLE != nullptr) {
    //     for (int i=0; i<MAP_ROWS; i++) {
    //         for (int j=0; j<MAP_COLS; j++) {
    //             DEBUG_CONSOLE->cprintf("%d", construct_array[i][j]);
    //         }
    //     }
    // }
    // add creature points to blocking array
    for (int i=0; i<CreatureList.size(); i++) {
        blocking_array[CreatureList[i]->pos.first][CreatureList[i]->pos.second] = 1;
    }
    for (int t=0; t<TownList.size(); t++) {
        for (int i=0; i<TownList[t].CreatureList.size(); i++) {
            blocking_array[TownList[t].CreatureList[i]->pos.first][TownList[t].CreatureList[i]->pos.second] = 1;
        }
    }
}

void MapSlice::Update() {
    // update creatures
    for (int i=0; i<CreatureList.size(); i++) {
        cleanBlockingArray();
        updateBlockingArray();
        CreatureList[i]->Update();
    }
    // update creatures in towns
    for (int t=0; t<TownList.size(); t++) {
        for (int i=0; i<TownList[t].CreatureList.size(); i++) {
            cleanBlockingArray();
            updateBlockingArray();
            TownList[t].CreatureList[i]->Update();
        }
    }
}



//////////////////////////////////////////////////////
