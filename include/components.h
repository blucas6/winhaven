#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <math.h>
#include <vector>
#include "enums.h"
#include "config.h"
#include "ConsoleLogger.h"
#include "enums.h"
#include "being.h"
#include "construct.h"
#include "astar.h"

// Base component class ////////////////
class Component {
    public:
        Component(ComponentID _ID, CConsoleLoggerEx *_debugconsole);
        ComponentID ID;
        CConsoleLoggerEx *DEBUG_CONSOLE;

        virtual void Update(Being *self);
};

// Handle any movement, positioning ////
class Movement_C : public Component {
    public:
        Movement_C(std::pair<int,int> *_pos, CConsoleLoggerEx *_debugconsole, int blockingLevel);

        Astar astar;

        std::pair<int,int> PathFailedCounter = {10,10};     // [MAX TRIES, CURRENT]

        void Update(Being *self);
        void Wander(Being *self);
        void Move(Being *self);
};

// Handle job actions ///////////////////
class Job_C : public Component {
    public:
        Job_C(Jobs _job, CConsoleLoggerEx *_debugconsole);
        Jobs job;               // job type

        void Update(Being *self);
};

// Handle build actions ////////////////
class Build_C : public Component {
    public:
        Build_C(std::vector<Construct*> *buildingListp, CConsoleLoggerEx *_debugconsole);
        std::vector<Construct*> *worldBuildListPtr = nullptr;         // pointer to construction list
        std::string name;
        std::pair<int,int> BuildLocation;
        std::pair<int,int> BuildSize;
        std::pair<int,int> maxBuildsz;
        std::vector<std::pair<int,int>> WallPoints;
        std::vector<std::pair<int,int>> FloorPoints;
        bool needtobuild = false;           // true when build is ready to start building
        Room room;
        PointStruct material;

        std::pair<int,int> max_shimmy = {10, 0};

        void Update(Being *self);
        bool init(Jobs Job);
        void findWallPoints();
        void findFloorPoints();
        bool placeBlock(Being *self);
        bool isBuildValid();
};

#endif