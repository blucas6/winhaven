#ifndef ENUMS_H
#define ENUMS_H

#include <vector>

enum Thoughts {
    NO_THOT,
    RELAX,
    BUILD
};

enum States {
    IDLE,
    MOVE,
    WANDER,
};

enum Jobs {
    NOJOB,
    FARMER,
    BREWER,
    PRIEST
};

enum ComponentID {
    MOVE_C,
    JOB_C,
    BUILD_C
};

enum Furniture {
    TABLE,
    CHAIR
};

#endif