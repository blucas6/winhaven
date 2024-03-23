#ifndef TOWN_H
#define TOWN_H

#include "being.h"
#include <vector>

class Town {
    public:
        Town();
        int ID;
        std::vector<Being*> CreatureList;               // all creatures in town

};


#endif