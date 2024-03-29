#include "components.h"

// BASE CLASS //////////////
Component::Component(ComponentID _ID, CConsoleLoggerEx *_debugconsole) {
    ID = _ID;
    DEBUG_CONSOLE = _debugconsole;
}

void Component::Update(Being *self) {
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[component]\tWARNING -- base component updater being used\n");
}
////////////////////////////

// MOVEMENT COMPONENT //////
Movement_C::Movement_C(std::pair<int,int> *_pos, CConsoleLoggerEx *_debugconsole, int blockingLevel) : Component (MOVE_C, _debugconsole), astar(_debugconsole) {
    astar.block_level = blockingLevel;
    astar.useDiag = true;
    astar.useMomentum = false;
}

void Movement_C::Update(Being *self) {
    if (self->moveto.first != -1 && self->moveto.second != -1) self->state = MOVE;
        // put in move state
    if (self->state == WANDER) Wander(self);
    else if (self->state == MOVE) Move(self);
}

void Movement_C::Wander(Being *self) {
    int choices[9][2] = { {0,0}, {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };
    int c = rand() % 9;
    std::pair<int,int> p;
    p.first = self->pos.first + choices[c][0];
    p.second = self->pos.second + choices[c][1];
    if (self->currBlockingArray != nullptr) {
        if (astar.isValid(p) && astar.isUnBlocked((*self->currBlockingArray), p)) {
            self->pos.first += choices[c][0];
            self->pos.second += choices[c][1];
        }
    }
}

void Movement_C::Move(Being *self) {
    if (self->moveto.first != -1 && self->moveto.second != -1) {
        std::vector<std::pair<int,int>> Path;
        if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[move c]\tFrom: (%d,%d) to (%d,%d)\n", self->pos.first, self->pos.second, self->moveto.first, self->moveto.second);
        // for (int i=0; i<MAP_ROWS; i++) {
        //     for (int j=0; j<MAP_COLS; j++) {
        //         DEBUG_CONSOLE->cprintf("%d", (*self).currBlockingArray[i][j]);
        //     }
        // }
        if (astar.astar(*(self->currBlockingArray), self->pos, self->moveto, Path)) {
            // path success
            if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[move c]\tMoving being (%d,%d)\n", Path[0].first, Path[0].second);
            self->pos.first = Path[Path.size()-2].first;
            self->pos.second = Path[Path.size()-2].second;
        } else {
            if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[move c]\tAstar failure!!\n");
            // after a max amount of tries put the being back into a new state
            if (PathFailedCounter.second - 1 <= 0) {
                self->thought = NO_THOT;
                PathFailedCounter.second = PathFailedCounter.first;
            } else 
                PathFailedCounter.second--;
        }
        self->moveto.first = -1;
        self->moveto.second = -1;
    }
}

////////////////////////////

// JOB COMPONENT ///////////
Job_C::Job_C(Jobs _job, CConsoleLoggerEx *_debugconsole) : Component(JOB_C, _debugconsole) {
    job = _job;
}

void Job_C::Update(Being *self) {

}

////////////////////////////

Build_C::Build_C(std::vector<Construct*> *buildingListp, std::vector<std::vector<Land>> *_landPiecesPtr, CConsoleLoggerEx *_debugconsole) : Component(BUILD_C, _debugconsole), room("room", _debugconsole) {
    worldBuildListPtr = buildingListp;
    landPiecesPtr = _landPiecesPtr;
    material.color = FG_YELLOW;
    material.glyph = '0';
    material.blockingLevel = 1;
}

// Checks if build is overlapping with anything
bool Build_C::isBuildValid() {
    // used when builds are shimmied
    for (int i=0; i<WallPoints.size(); i++) {
        if (WallPoints[i].first < 0 || WallPoints[i].second < 0 || WallPoints[i].first >= MAP_ROWS || WallPoints[i].second >= MAP_COLS) {
            return false;
        }
    }
    for (int i=0; i<GardenPoints.size(); i++) {
        if (GardenPoints[i].first < 0 || GardenPoints[i].second < 0 || GardenPoints[i].first >= MAP_ROWS || GardenPoints[i].second >= MAP_COLS) {
            return false;
        }
    }

    // go through all builds
    for (const Construct* el: *worldBuildListPtr) {
        // cycle through all wall pts in construct
        for (int b=0; b<el->wallPoints.size(); b++) {
            for (int i=0; i<WallPoints.size(); i++) {
                if (el->wallPoints[b].first == WallPoints[i].first && el->wallPoints[b].second == WallPoints[i].second) {
                    return false;
                }
            }
        }
        // go through floor points (builds could be inside eachother!)
        for (int b=0; b<el->floorPoints.size(); b++) {
            for (int i=0; i<FloorPoints.size(); i++) {
                if (el->floorPoints[b].first == FloorPoints[i].first && el->floorPoints[b].second == FloorPoints[i].second) {
                    return false;
                }
            }
        }
        // go through garden points
        for (int b=0; b<el->gardenPoints.size(); b++) {
            for (int i=0; i<GardenPoints.size(); i++) {
                if (el->gardenPoints[b].first == GardenPoints[i].first && el->gardenPoints[b].second == GardenPoints[i].second) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Initialize a build type depending on the job
bool Build_C::init(Jobs job) {
    switch (job) {
        case FARMER:
            name = "Farm";
            maxBuildsz = FARM_SZ_MAX;
            needGarden = true;
            maxGardensz = FARM_GARDEN_SZ_MAX;
            break;
        case BREWER:
            name = "Alehouse";
            maxBuildsz = ALEHOUSE_SZ_MAX;
            break;
        case PRIEST:
            name = "Church";
            maxBuildsz = CHURCH_SZ_MAX;
            break;
        default:
            name = "House";
            maxBuildsz = HOUSE_SZ_MAX;
            break;
    }

    BuildLocation.first = rand() % (MAP_ROWS-maxBuildsz-1);
    BuildLocation.second = rand() % (MAP_COLS-maxBuildsz-1);
    BuildSize.first = MINIMUM_BUILD_SZ + rand() % maxBuildsz;
    BuildSize.second = MINIMUM_BUILD_SZ + rand() % maxBuildsz;
    findBuildPoints();
    if (!isBuildValid()) {
        bool valid = false;
        for (int m=0; m<max_shimmy.first; m++) {
            for (int r=-1; r<2; r++) {
                for (int c=-1; c<2; c++) {
                    BuildLocation.first += r+m;
                    BuildLocation.second += c+m;
                    findBuildPoints();
                    if (isBuildValid()) {
                        valid = true;
                        break;
                    }
                }
                if (valid) break;
            }
            if (valid) break;
        }
        if (!isBuildValid()) {
            if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tFailed to pick build\n");
            return false;
        }
    }
    needtobuild = true;
    room.width = BuildSize.first;
    room.height = BuildSize.second;
    room.type = name;
    room.wallPoints = WallPoints;
    room.floorPoints = FloorPoints;
    room.gardenPoints = GardenPoints;
    Construct *roomptr = &room;
    worldBuildListPtr->push_back(roomptr);
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tBuild picked at (%d,%d)\n", BuildLocation.first, BuildLocation.second);
    return true;
}

void Build_C::Update(Being *self) {
    // Check if we have built yet
    if (needtobuild && self->thought == BUILD) {
        if (!placeBlock(self)) {
            // build is done
            self->thought = NO_THOT;
            // remove BUILD thought
            int ind = -1;
            for (int i=0; i<self->thought_list.size(); i++) {
                if (self->thought_list[i] == BUILD) ind = i;
            }
            if (ind != -1) self->thought_list.erase(self->thought_list.begin() + ind);
        }
    }
}

// finds all the points for walls
void Build_C::findBuildPoints() {
    WallPoints.clear();
    GardenPoints.clear();
    FloorPoints.clear();

    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tTrying (%d,%d) w:%d h:%d\n", BuildLocation.first, BuildLocation.second, BuildSize.first, BuildSize.second);
    
    // top row
    for (int i=0; i<BuildSize.first; i++) {
        WallPoints.push_back(std::make_pair(BuildLocation.first + 0, BuildLocation.second + i));
    }

    // right col
    for (int i=1; i<BuildSize.second-1; i++) {
        WallPoints.push_back(std::make_pair(BuildLocation.first + i, BuildLocation.second + BuildSize.first - 1 + 0));

    }
    // bottom row
    for (int i=0; i<BuildSize.first; i++) {
        WallPoints.push_back(std::make_pair(BuildLocation.first + BuildSize.second - 1 + 0, BuildLocation.second + i));
    }

    // left col
    for (int i=1; i<BuildSize.second-1; i++) {
        WallPoints.push_back(std::make_pair(BuildLocation.first + i, BuildLocation.second + 0));
    }

    // find floor points
    for (int i=BuildLocation.first+1; i<BuildLocation.first+BuildSize.second-1; i++) {
        for (int j=BuildLocation.second+1; j<BuildLocation.second+BuildSize.first-1; j++) {
            FloorPoints.push_back(std::make_pair(i,j));
        }
    }

    if (needGarden) {
        std::pair<int,int> buildpt = WallPoints[rand() % WallPoints.size()];
        int side = -1;
        // top row
        for (int i=0; i<BuildSize.first; i++) {
            std::pair<int,int> top = {BuildLocation.first + 0, BuildLocation.second + i};
            if (buildpt == top) side = 1;
        }
        // bottom row
        for (int i=0; i<BuildSize.first; i++) {
            std::pair<int,int> bot = {BuildLocation.first + BuildSize.second - 1 + 0, BuildLocation.second + i};
            if (buildpt == bot) side = 2;
        }
        // left col
        for (int i=1; i<BuildSize.second-1; i++) {
            std::pair<int,int> left = {BuildLocation.first + i, BuildLocation.second + 0};
            if (buildpt == left) side = 3;
        }            
        // right col
        for (int i=1; i<BuildSize.second-1; i++) {
            std::pair<int,int> right = {BuildLocation.first + i, BuildLocation.second + BuildSize.first - 1 + 0};
            if (buildpt == right) side = 4;
        }
        if (side != -1) {
            garden_size = { FARM_GARDEN_SZ_MIN + rand() % FARM_GARDEN_SZ_MAX, FARM_GARDEN_SZ_MIN + rand() % FARM_GARDEN_SZ_MAX};
            // garden is on top
            if (side == 1) garden_pos = {buildpt.first - 1 - garden_size.first, buildpt.second};
            // bottom side
            else if (side == 2) garden_pos = {buildpt.first + 1 + garden_size.first, buildpt.second};
            // left side
            else if (side == 3) garden_pos = {buildpt.first, buildpt.second - 1 - garden_size.second};
            // right side
            else if (side == 4) garden_pos = {buildpt.first, buildpt.second + 1 + garden_size.second};
            
            // fill in the rest of the garden
            for (int i=0; i<garden_size.first; i++) {
                for (int j=0; j<garden_size.second; j++) {
                    GardenPoints.push_back(std::make_pair(garden_pos.first+i, garden_pos.second+j));
                }
            }
        }
        if (GardenPoints.size() > 0) {
            if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tGarden picked! pts (%d)\n", GardenPoints.size());
        }
    }
}

// checks if less than 1 sq away
bool distCheck(std::pair<int,int> bpos, std::pair<int,int> cpos) {
    return (sqrt((bpos.first - cpos.first)*(bpos.first - cpos.first) + (bpos.second - cpos.second)*(bpos.second - cpos.second)) < 1.5) ? true : false;
}

// adds a wall to the map (construction list)
// returns false when done building
bool Build_C::placeBlock(Being *self) {
    if (WallPoints.size() > 0) {
        if (WallPoints[0].first < MAP_ROWS && WallPoints[0].second < MAP_COLS && WallPoints[0].first > -1 && WallPoints[0].second > -1) {
            // check if being is next to point
            if (distCheck(self->pos, WallPoints[0]) && self->currConstructArray != nullptr) {
                // define points for material
                material.pos.first = WallPoints[0].first;
                material.pos.second = WallPoints[0].second;
                // add material point to room
                PointStruct wall = material;
                room.PointStructs.push_back(wall);
                (*self->currConstructArray)[WallPoints[0].first][WallPoints[0].second] = 1;
                WallPoints.erase(WallPoints.begin());
                if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tPlaced wall!\n");
                return true;
            } else {
                // being is not at point - moveto it
                self->moveto.first = WallPoints[0].first;
                self->moveto.second = WallPoints[0].second;
                if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tMoving to wall\n");
                return true;
            }
        } else {
            // failure to finish the room - offscreen error
            if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tError - wall out of range\n");
            needtobuild = false;
            return false;
        }
    }
    if (GardenPoints.size() > 0) {
        if (GardenPoints[0].first < MAP_ROWS && GardenPoints[0].second < MAP_COLS && GardenPoints[0].first > -1 && GardenPoints[0].second > -1) {
            if (distCheck(self->pos, GardenPoints[0]) && landPiecesPtr != nullptr) {
                (*landPiecesPtr)[GardenPoints[0].first][GardenPoints[0].second].ungrowth();
                GardenPoints.erase(GardenPoints.begin());
                if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tPlaced garden pt!\n");
                return true;
            } else {
                self->moveto.first = GardenPoints[0].first;
                self->moveto.second = GardenPoints[0].second;
                if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tMoving to garden\n");
                return true;
            }
        } else {
            if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tError - garden out of range\n");
            needtobuild = false;
            return false;
        }
    }
    // finished build
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tFINISHING ROOM\n");
    needtobuild = false;
    room.Finish(needGarden);
    return false; 
}