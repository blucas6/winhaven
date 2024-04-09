#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <math.h>
#include <vector>
#include <memory>
#include "enums.h"
#include "config.h"
#include "ConsoleLogger.h"
#include "being.h"
#include "construct.h"
#include "astar.h"
#include "map.h"

// Base component class ////////////////
class Component {
    public:
        Component(ComponentID _ID, CConsoleLoggerEx *_debugconsole);
        virtual ~Component() {};
        ComponentID ID;
        CConsoleLoggerEx *DEBUG_CONSOLE = nullptr;

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
        DoorAction CheckForDoor(Being *self, std::pair<int,int> pt);
        void PathFailure(Being *self);
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
        Build_C(std::vector<std::shared_ptr<Construct>> *buildingListp, std::vector<std::vector<Land>> *_landPiecesPtr, CConsoleLoggerEx *_debugconsole);
        std::vector<std::shared_ptr<Construct>> *mapBuildListPtr = nullptr;         // pointer to construction list
        std::vector<std::vector<Land>> *landPiecesPtr = nullptr;    // pointer to land pieces
        std::string name;
        std::pair<int,int> BuildLocation;
        std::pair<int,int> BuildSize;
        int maxBuildsz;
        std::vector<std::pair<int,int>> WallPoints;
        std::vector<std::pair<int,int>> FloorPoints;
        std::vector<std::pair<int,int>> GardenPoints;
        bool needtobuild = false;           // true when build is ready to start building
        Room room;
        bool needGarden = false;
        int maxGardensz;
        std::pair<int,int> garden_size;
        std::pair<int,int> garden_pos;

        std::pair<int,int> max_shimmy = {10, 0};

        void Update(Being *self);
        bool init(Being *self, Jobs Job);
        void findBuildPoints();
        bool placeBlock(Being *self);
        bool isBuildValid();
};

#endif