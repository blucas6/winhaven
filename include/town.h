#ifndef TOWN_H
#define TOWN_H

#include <vector>
#include <string>
#include "being.h"

class Town {
    public:
        Town();
        int ID;
        std::vector<Being*> CreatureList;               // all creatures in town

};


#endif