#include <string>

#include "construct.h"
#include "ConsoleLogger.h"

Room::Room(std::string _name) : Construct(_name) {
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
    door.glyph = '+';
    door.color = FG_DARKYELLOW;
    door.blockingLevel = 0;
}

void Room::Finish(CConsoleLoggerEx *debugconsole) {
    std::vector<int> door_ind; 
    for(int i=1; i<width-1; i++) {
        PointStructs[i].glyph = wall_h.glyph;
        PointStructs[i].color = wall_h.color;
        PointStructs[i+width].glyph = wall_h.glyph;
        PointStructs[i+width].color = wall_h.color;
        door_ind.push_back(i);
        door_ind.push_back(i+width);
    }
    for(int j=width+width; j<PointStructs.size(); j++) {
        PointStructs[j].glyph = wall_v.glyph;
        PointStructs[j].color = wall_v.color;
        door_ind.push_back(j);
    }
    int ch = rand() % door_ind.size();
    PointStructs[door_ind[ch]].glyph = door.glyph;
    PointStructs[door_ind[ch]].color = door.color;
    PointStructs[door_ind[ch]].blockingLevel = door.blockingLevel;
}


Construct::Construct(std::string _type) {
    type = _type;
}

void Construct::Update() {

}