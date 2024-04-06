#include <string>

#include "construct.h"

Construct::Construct(std::string _type, CConsoleLoggerEx *_debugconsole) {
    type = _type;
    DEBUG_CONSOLE = _debugconsole;
}

void Construct::Update() {

}

Room::Room(std::string _name, CConsoleLoggerEx *_debugconsole) : Construct(_name, _debugconsole) {
    // corner.glyph = '0';
    corner_tl.glyph = char(201);
    corner_tl.color = FG_YELLOW;
    corner_tl.blockingLevel = 1;

    corner_tr.glyph = char(187);
    corner_tr.color = FG_YELLOW;
    corner_tr.blockingLevel = 1;
    
    corner_bl.glyph = char(200);
    corner_bl.color = FG_YELLOW;
    corner_bl.blockingLevel = 1;

    corner_br.glyph = char(188);
    corner_br.color = FG_YELLOW;
    corner_br.blockingLevel = 1;

    // wall_h.glyph = '-';
    wall_h.glyph = char(205);
    wall_h.color = FG_YELLOW;
    wall_h.blockingLevel = 1;

    // wall_v.glyph = '|';
    wall_v.glyph = char(186);
    wall_v.color = FG_YELLOW;
    wall_v.blockingLevel = 1;

    roof.glyph = (char)176;
    roof.color = FG_DARKYELLOW;

    floor.glyph = '.';
    floor.color = FG_DARKYELLOW;
    floor.blockingLevel = 0;

    door.glyph = '+';
    door.color = FG_DARKYELLOW;
    door.blockingLevel = 0;

    table.blockingLevel = 0;
    table.color = FG_YELLOW;
    table.glyph = (char)210;

    chair.blockingLevel = 0;
    chair.color = FG_YELLOW;
    chair.glyph = (char)170;
}

void Room::MakeWalls() {
    // add corners
    PointStructs[0].glyph = corner_tl.glyph;
    PointStructs[0].color = corner_tl.color;
    PointStructs[0].blockingLevel = corner_tl.blockingLevel;
    
    PointStructs[width-1].glyph = corner_tr.glyph;
    PointStructs[width-1].color = corner_tr.color;
    PointStructs[width-1].blockingLevel = corner_tr.blockingLevel;

    PointStructs[width+height-2].glyph = corner_br.glyph;
    PointStructs[width+height-2].color = corner_br.color;
    PointStructs[width+height-2].blockingLevel = corner_br.blockingLevel;
    
    PointStructs[width+width+height-3].glyph = corner_bl.glyph;
    PointStructs[width+width+height-3].color = corner_bl.color;
    PointStructs[width+width+height-3].blockingLevel = corner_bl.blockingLevel;

    std::vector<int> door_ind; 
    // find door indices + make walls
    for(int i=1; i<width-1; i++) {
        PointStructs[i].glyph = wall_h.glyph;
        PointStructs[i].color = wall_h.color;
        PointStructs[i+width+height-2].glyph = wall_h.glyph;
        PointStructs[i+width+height-2].color = wall_h.color;
        door_ind.push_back(i);
        door_ind.push_back(i+width+height-2);
    }
    for(int j=width; j<width+height-2; j++) {
        PointStructs[j].glyph = wall_v.glyph;
        PointStructs[j].color = wall_v.color;
        PointStructs[j+width+height-2].glyph = wall_v.glyph;
        PointStructs[j+width+height-2].color = wall_v.color;
        door_ind.push_back(j);
        door_ind.push_back(j+width+height-2);
    }
    // make one of the wall pts a door
    int ch = rand() % door_ind.size();
    PointStructs[door_ind[ch]].glyph = door.glyph;
    PointStructs[door_ind[ch]].color = door.color;
    PointStructs[door_ind[ch]].blockingLevel = door.blockingLevel;
}

std::vector<PointStruct> Room::AddFlooring() {
    // add flooring
    std::vector<PointStruct> flooring;
    for (int i=0; i<floorPoints.size(); i++) {
        // use floor object to create copies
        floor.pos.first = floorPoints[i].first;
        floor.pos.second = floorPoints[i].second;
        flooring.push_back(floor);
    }
    return flooring;
}

std::vector<PointStruct> Room::AddFurniture(Furniture type, std::vector<PointStruct> flooring, int pt) {
    switch (type) {
        case TABLE:
            flooring[pt].blockingLevel = table.blockingLevel;
            flooring[pt].color = table.color;
            flooring[pt].glyph = table.glyph;
            break;
        case CHAIR:
            flooring[pt].blockingLevel = chair.blockingLevel;
            flooring[pt].color = chair.color;
            flooring[pt].glyph = chair.glyph;
            break;
        default:
            break;
    }
    return flooring;
}

void Room::Finish(bool isFence) {

    MakeWalls();
    std::vector<PointStruct> flooring = AddFlooring();

    // furniture
    int ch = rand() % flooring.size();
    flooring = AddFurniture(TABLE, flooring, ch);

    std::pair<int,int> trypt;
    int chair_pt = -1;
    // try left
    trypt.first = flooring[ch].pos.first-1;
    trypt.second = flooring[ch].pos.second;
    chair_pt = ValidFloorPt(flooring, trypt);
    // try right
    if (chair_pt < 0) {
        trypt.first = flooring[ch].pos.first+1;
        trypt.second = flooring[ch].pos.second; 
        chair_pt = ValidFloorPt(flooring, trypt);
    }
    // try top
    if (chair_pt < 0) {
        trypt.first = flooring[ch].pos.first;
        trypt.second = flooring[ch].pos.second-1;
        chair_pt = ValidFloorPt(flooring, trypt); 
    }
    // try bottom
    if (chair_pt < 0) {
        trypt.first = flooring[ch].pos.first;
        trypt.second = flooring[ch].pos.second+1;
            chair_pt = ValidFloorPt(flooring, trypt);  
    }
    if (chair_pt >= 0) {
        flooring = AddFurniture(CHAIR, flooring, chair_pt);
    }   

    // connect flooring to pointstructs array
    PointStructs.insert(PointStructs.end(), flooring.begin(), flooring.end());
}

int Room::ValidFloorPt(std::vector<PointStruct> flooring, std::pair<int,int> pt) {
    for (int i=0; i<flooring.size(); i++) {
        if (pt == flooring[i].pos) {
            return i;
        }
    }
    return -1;
}