#include <string>

#include "construct.h"

Construct::Construct(std::string _type, CConsoleLoggerEx *_debugconsole) {
    type = _type;
    DEBUG_CONSOLE = _debugconsole;
}

void Construct::Update() {

}

Room::Room(std::string _name, CConsoleLoggerEx *_debugconsole) : Construct(_name, _debugconsole) {
    corner.glyph = '0';
    corner.color = FG_YELLOW;
    wall_h.glyph = '-';
    wall_h.color = FG_YELLOW;
    wall_h.blockingLevel = 1;
    wall_v.glyph = '|';
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
}

void Room::Finish(bool isFence) {
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

    // add flooring
    std::vector<PointStruct> flooring;
    for (int i=0; i<floorPoints.size(); i++) {
        floor.pos.first = floorPoints[i].first;
        floor.pos.second = floorPoints[i].second;
        flooring.push_back(floor);
    }
    
    // furniture
    ch = rand() % flooring.size();
    flooring[ch].blockingLevel = table.blockingLevel;
    flooring[ch].color = table.color;
    flooring[ch].glyph = table.glyph;
    ch = rand() % flooring.size();
    flooring[ch].blockingLevel = table.blockingLevel;
    flooring[ch].color = table.color;
    flooring[ch].glyph = table.glyph;

    // connect flooring to pointstructs array
    PointStructs.insert(PointStructs.end(), flooring.begin(), flooring.end());
}