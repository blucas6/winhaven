#include "construct.h"

PointStruct::PointStruct() {
    name = "???";
    glyph = '?';
    color = FG_MAGENTA;
    blockingLevel = 0;
    type = PT_UNKNOWN;
}

PointStruct::PointStruct(CConsoleLoggerEx *_debugconsole) {
    name = "???";
    glyph = '?';
    color = FG_MAGENTA;
    blockingLevel = 0;
    DEBUG_CONSOLE = _debugconsole;
    type = PT_UNKNOWN;
}

PointStruct::PointStruct(std::string _name, char _glyph, int _color, int _blockingLevel, PT_Type _type, std::pair<int,int> _pos) {
    name = _name;
    glyph = _glyph;
    color = _color;
    blockingLevel = _blockingLevel;
    type = _type;
    pos = _pos;
}

void PointStruct::Use() {

}

void PointStruct::Open() {
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[PtSt]\tWarning - Using virtual Open method!\n");    
}

void PointStruct::Close() {

}

Material::Material(std::pair<int,int> _pos) : PointStruct("material", '0', FG_YELLOW, 1, PT_MATERIAL, _pos) {}

CornerTL::CornerTL(std::pair<int,int> _pos) : PointStruct("Corner wall", char(201), FG_YELLOW, 1, PT_CORNER_TL, _pos) {}

CornerTR::CornerTR(std::pair<int,int> _pos) : PointStruct("Corner wall", char(187), FG_YELLOW, 1, PT_CORNER_TR, _pos) {}

CornerBL::CornerBL(std::pair<int,int> _pos) : PointStruct("Corner wall", char(200), FG_YELLOW, 1, PT_CORNER_BL, _pos) {}

CornerBR::CornerBR(std::pair<int,int> _pos) : PointStruct("Corner wall", char(188), FG_YELLOW, 1, PT_CORNER_BR, _pos) {}

Wall_H::Wall_H(std::pair<int,int> _pos) : PointStruct("wall", char(205), FG_YELLOW, 1, PT_WALL_H, _pos) {}

Wall_V::Wall_V(std::pair<int,int> _pos) : PointStruct("wall", char(186), FG_YELLOW, 1, PT_WALL_V, _pos) {}

Floor::Floor(std::pair<int,int> _pos) : PointStruct("floor", '.', FG_DARKYELLOW, 0, PT_FLOOR, _pos) {}

Door_H::Door_H(std::pair<int,int> _pos) : PointStruct("door", '+', FG_DARKYELLOW, DOOR_PT_LEVEL, PT_DOOR_H, _pos) {
    open_glyph = '|';
    close_glyph = '+';
}

void Door_H::Open() {
    glyph = open_glyph;
    blockingLevel = 0;
}

void Door_H::Close() {
    glyph = close_glyph;
    blockingLevel = DOOR_PT_LEVEL;
}

Door_V::Door_V(std::pair<int,int> _pos) : PointStruct("door", '+', FG_DARKYELLOW, DOOR_PT_LEVEL, PT_DOOR_V, _pos) {
    open_glyph = '-';
    close_glyph = '+';
}

void Door_V::Open() {
    glyph = open_glyph;
    blockingLevel = 0;
}

void Door_V::Close() {
    glyph = close_glyph;
    blockingLevel = DOOR_PT_LEVEL;
}

Table::Table(std::pair<int,int> _pos) : PointStruct("table", (char)210, FG_YELLOW, 0, PT_TABLE, _pos) {}

Chair::Chair(std::pair<int,int> _pos) : PointStruct("chair", (char)170, FG_YELLOW, 0, PT_CHAIR, _pos) {}

Construct::Construct(std::string _type, CConsoleLoggerEx *_debugconsole) {
    type = _type;
    DEBUG_CONSOLE = _debugconsole;
}

void Construct::Update() {

}

void Construct::addConstruct(std::shared_ptr<PointStruct> pt, std::pair<int,int> pos) {
    (*currPointStruct_Array)[pos.first][pos.second] = pt;
    (*currConstructArray)[pos.first][pos.second] = (*currPointStruct_Array)[pos.first][pos.second]->blockingLevel;
}

Room::Room(std::string _name, CConsoleLoggerEx *_debugconsole) : Construct(_name, _debugconsole) {
   
}

void Room::MakeWalls() {
    // add corners
    std::vector<std::pair<int,int>> pointsToUse = { wallPoints[0], wallPoints[width-1], wallPoints[width+height-2], wallPoints[width+width+height-3]};
    std::vector<std::shared_ptr<PointStruct>> corners = {std::make_shared<CornerTL>(pointsToUse[0]), std::make_shared<CornerTR>(pointsToUse[1]), std::make_shared<CornerBR>(pointsToUse[2]), std::make_shared<CornerBL>(pointsToUse[3])};
    for (int i=0; i<pointsToUse.size(); i++) {
        (*currPointStruct_Array)[pointsToUse[i].first][pointsToUse[i].second] = corners[i];
        (*currConstructArray)[pointsToUse[i].first][pointsToUse[i].second] = corners[i]->blockingLevel;
    }

    std::vector<int> door_ind; 
    // find door indices + make walls
    // horizontal walls
    for(int i=1; i<width-1; i++) {
        addConstruct(std::make_shared<Wall_H>(wallPoints[i]), wallPoints[i]);
        addConstruct(std::make_shared<Wall_H>(wallPoints[i+width+height-2]), wallPoints[i+width+height-2]);
        door_ind.push_back(i);
        door_ind.push_back(i+width+height-2);
    }
    // vertical walls
    for(int j=width; j<width+height-2; j++) {
        addConstruct(std::make_shared<Wall_V>(wallPoints[j]), wallPoints[j]);
        addConstruct(std::make_shared<Wall_V>(wallPoints[j+width+height-2]), wallPoints[j+width+height-2]);
        door_ind.push_back(j);
        door_ind.push_back(j+width+height-2);
    }

    //make one of the wall pts a door
    int ch = rand() % door_ind.size();
    std::pair<int,int> dpos = wallPoints[door_ind[ch]];
    if (door_ind[ch] < width) addConstruct(std::make_shared<Door_H>(dpos), dpos);
    else if (door_ind[ch] < width + height - 1) addConstruct(std::make_shared<Door_V>(dpos), dpos);
    else if (door_ind[ch] < width + height + width - 2) addConstruct(std::make_shared<Door_H>(dpos), dpos);
    else addConstruct(std::make_shared<Door_V>(dpos), dpos);
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[room]\tDoor is of type %d\n", (*currPointStruct_Array)[dpos.first][dpos.second]->type);    
}

std::vector<std::shared_ptr<PointStruct>> Room::AddFlooring() {
    // add flooring
    std::vector<std::shared_ptr<PointStruct>> flooring;
    for (int i=0; i<floorPoints.size(); i++) {
        // use floor object to create copies
        flooring.push_back(std::make_shared<Floor>(floorPoints[i]));
    }
    return flooring;
}

bool Room::Finish(bool isFence) {
    if (currPointStruct_Array == nullptr) {
        if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[room]\t**Error: No PointStruct array to point to!!**\n");    
        return false;
    }

    MakeWalls();

    std::vector<std::shared_ptr<PointStruct>> flooring = AddFlooring();

    // furniture
    int ch = rand() % flooring.size();
    std::pair<int,int> pos = flooring[ch]->pos;
    flooring[ch] = std::make_shared<Table>(pos);

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
        pos = flooring[chair_pt]->pos;
        flooring[chair_pt] = std::make_shared<Chair>(pos);
    }   

    // once done with the floor, add each structure to the pt_array
    for (std::shared_ptr<PointStruct> p : flooring) {
        addConstruct(p, p->pos);
    }

    return true;
}

int Room::ValidFloorPt(std::vector<std::shared_ptr<PointStruct>> flooring, std::pair<int,int> pt) {
    for (int i=0; i<flooring.size(); i++) {
        if (pt == flooring[i]->pos) {
            return i;
        }
    }
    return -1;
}