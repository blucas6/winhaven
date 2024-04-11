#ifndef ENUMS_H
#define ENUMS_H

enum Thoughts {
    NO_THOT,
    RELAX,
    BUILD,
    GOTO
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

enum DoorAction {
    DOOR_OPENED,
    NO_DOOR,
    DOOR_FAIL
};

enum PT_Type {
    PT_MATERIAL,
    PT_DOOR_H,
    PT_DOOR_V,
    PT_WALL_H,
    PT_WALL_V,
    PT_CORNER_TL,
    PT_CORNER_TR,
    PT_CORNER_BL,
    PT_CORNER_BR,
    PT_TABLE,
    PT_CHAIR,
    PT_FLOOR,
    PT_UNKNOWN
};

enum Construct_T {
    DEFAULT_CON,
    ROOM_CON
};

#endif