#include <string>

#include "construct.h"
#include "config.h"

PointStruct::PointStruct() {
    name = "???";
    glyph = '?';
    color = FG_MAGENTA;
    blockingLevel = 0;
}

PointStruct::PointStruct(CConsoleLoggerEx *_debugconsole) {
    name = "???";
    glyph = '?';
    color = FG_MAGENTA;
    blockingLevel = 0;
    DEBUG_CONSOLE = _debugconsole;
}

void PointStruct::Use() {

}

void PointStruct::Open() {
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[PtSt]\tWarning - Using virtual Open method!\n");    
}

void PointStruct::Close() {

}

Material::Material() {
    name = "material";
    glyph = '0';
    color = FG_YELLOW;
    blockingLevel = 1;
}

CornerTL::CornerTL() {
    name = "Corner wall";
    glyph = char(201);
    color = FG_YELLOW;
    blockingLevel = 1;
}

CornerTR::CornerTR() {
    name = "Corner wall";
    glyph = char(187);
    color = FG_YELLOW;
    blockingLevel = 1;
}

CornerBL::CornerBL() {
    name = "Corner wall";
    glyph = char(200);
    color = FG_YELLOW;
    blockingLevel = 1;
}

CornerBR::CornerBR() {
    name = "Corner wall";
    glyph = char(188);
    color = FG_YELLOW;
    blockingLevel = 1;
}

Wall_H::Wall_H() {
    name = "wall";
    glyph = char(205);
    color = FG_YELLOW;
    blockingLevel = 1;
}

Wall_V::Wall_V() {
    name = "wall";
    glyph = char(186);
    color = FG_YELLOW;
    blockingLevel = 1;
}

Floor::Floor() {
    name = "floor";
    glyph = '.';
    color = FG_DARKYELLOW;
    blockingLevel = 0;
}

Door_H::Door_H() {
    name = "door";
    glyph = '+';
    open_glyph = '|';
    close_glyph = '+';
    color = FG_DARKYELLOW;
    blockingLevel = DOOR_PT_LEVEL;
}

Door_H::Door_H(std::pair<int,int> _pos) {
    pos = _pos;
    name = "door";
    glyph = '+';
    open_glyph = '|';
    close_glyph = '+';
    color = FG_DARKYELLOW;
    blockingLevel = DOOR_PT_LEVEL;
}

void Door_H::Open() {
    glyph = open_glyph;
    blockingLevel = 0;
}

void Door_H::Close() {
    glyph = close_glyph;
    blockingLevel = DOOR_PT_LEVEL;
}

Door_V::Door_V() {
    name = "door";
    glyph = '+';
    open_glyph = '-';
    close_glyph = '+';
    color = FG_DARKYELLOW;
    blockingLevel = DOOR_PT_LEVEL;
}

Door_V::Door_V(std::pair<int,int> _pos) {
    pos = _pos;
    name = "door";
    glyph = '+';
    open_glyph = '-';
    close_glyph = '+';
    color = FG_DARKYELLOW;
    blockingLevel = DOOR_PT_LEVEL;
}

void Door_V::Open() {
    glyph = open_glyph;
    blockingLevel = 0;
}

void Door_V::Close() {
    glyph = close_glyph;
    blockingLevel = DOOR_PT_LEVEL;
}

Table::Table() {
    name = "table";
    glyph = (char)210;
    color = FG_YELLOW;
    blockingLevel = 0;
}

Chair::Chair() {
    name = "chair";
    glyph = (char)170;
    color = FG_YELLOW;
    blockingLevel = 0;
}


Construct::Construct(std::string _type, CConsoleLoggerEx *_debugconsole) {
    type = _type;
    DEBUG_CONSOLE = _debugconsole;
}

void Construct::Update() {

}

Room::Room(std::string _name, CConsoleLoggerEx *_debugconsole) : Construct(_name, _debugconsole) {
   
}

void Room::MakeWalls() {
    // add corners
    CornerTL *ctl = new CornerTL();
    CornerTR *ctr = new CornerTR();
    CornerBL *cbl = new CornerBL();
    CornerBR *cbr = new CornerBR();

    ctl->pos = Structures[0]->pos;
    delete Structures[0];
    Structures[0] = ctl;

    ctr->pos = Structures[width-1]->pos;
    delete Structures[width-1];
    Structures[width-1] = ctr;

    cbr->pos = Structures[width+height-2]->pos;
    delete Structures[width+height-2];
    Structures[width+height-2] = cbr;

    cbl->pos = Structures[width+width+height-3]->pos;
    delete Structures[width+width+height-3];
    Structures[width+width+height-3] = cbl;


    std::vector<int> door_ind; 
    // find door indices + make walls
    // horizontal walls
    for(int i=1; i<width-1; i++) {
        Wall_H *wallh = new Wall_H();
        wallh->pos = Structures[i]->pos;
        delete Structures[i];
        Structures[i] = wallh;
        wallh = new Wall_H();
        wallh->pos = Structures[i+width+height-2]->pos;
        delete Structures[i+width+height-2];
        Structures[i+width+height-2] = wallh;
        door_ind.push_back(i);
        door_ind.push_back(i+width+height-2);
    }
    // vertical walls
    for(int j=width; j<width+height-2; j++) {
        Wall_V *wallv = new Wall_V();
        wallv->pos = Structures[j]->pos;
        delete Structures[j];
        Structures[j] = wallv;
        wallv = new Wall_V();
        wallv->pos = Structures[j+width+height-2]->pos;
        delete Structures[j+width+height-2];
        Structures[j+width+height-2] = wallv;
        door_ind.push_back(j);
        door_ind.push_back(j+width+height-2);
    }

    //make one of the wall pts a door
    int ch = rand() % door_ind.size();
    delete Structures[door_ind[ch]];
    if (ch < width) Structures[door_ind[ch]] = new Door_H(Structures[door_ind[ch]]->pos);
    else if (ch < width + height - 1) Structures[door_ind[ch]] = new Door_V(Structures[door_ind[ch]]->pos);
    else if (ch < width + height + width - 2) Structures[door_ind[ch]] = new Door_H(Structures[door_ind[ch]]->pos);
    else Structures[door_ind[ch]] = new Door_V(Structures[door_ind[ch]]->pos);

    (*currConstructArray)[Structures[door_ind[ch]]->pos.first][Structures[door_ind[ch]]->pos.second] = Structures[door_ind[ch]]->blockingLevel;
}

std::vector<PointStruct*> Room::AddFlooring() {
    // add flooring
    std::vector<PointStruct*> flooring;
    for (int i=0; i<floorPoints.size(); i++) {
        // use floor object to create copies
        Floor *floor = new Floor();
        floor->pos = floorPoints[i];
        flooring.push_back(floor);
    }
    return flooring;
}

void Room::Finish(bool isFence) {

    MakeWalls();

    std::vector<PointStruct*> flooring = AddFlooring();

    // furniture
    int ch = rand() % flooring.size();
    Table *table = new Table();
    table->pos = flooring[ch]->pos;
    delete flooring[ch];
    flooring[ch] = table;

    std::pair<int,int> trypt;
    int chair_pt = -1;
    // try left
    trypt.first = flooring[ch]->pos.first-1;
    trypt.second = flooring[ch]->pos.second;
    chair_pt = ValidFloorPt(flooring, trypt);
    // try right
    if (chair_pt < 0) {
        trypt.first = flooring[ch]->pos.first+1;
        trypt.second = flooring[ch]->pos.second; 
        chair_pt = ValidFloorPt(flooring, trypt);
    }
    // try top
    if (chair_pt < 0) {
        trypt.first = flooring[ch]->pos.first;
        trypt.second = flooring[ch]->pos.second-1;
        chair_pt = ValidFloorPt(flooring, trypt); 
    }
    // try bottom
    if (chair_pt < 0) {
        trypt.first = flooring[ch]->pos.first;
        trypt.second = flooring[ch]->pos.second+1;
            chair_pt = ValidFloorPt(flooring, trypt);  
    }
    if (chair_pt >= 0) {
        Chair *chair = new Chair();
        chair->pos = flooring[chair_pt]->pos;
        delete flooring[chair_pt];
        flooring[chair_pt] = chair;
    }   

    // connect flooring to pointstructs array
    Structures.insert(Structures.end(), flooring.begin(), flooring.end());

}

int Room::ValidFloorPt(std::vector<PointStruct*> flooring, std::pair<int,int> pt) {
    for (int i=0; i<flooring.size(); i++) {
        if (pt == flooring[i]->pos) {
            return i;
        }
    }
    return -1;
}