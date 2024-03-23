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

Build_C::Build_C(std::vector<Construct*> *buildingListp, CConsoleLoggerEx *_debugconsole) : Component(BUILD_C, _debugconsole), room("room") {
    worldBuildListPtr = buildingListp;
    material.color = FG_YELLOW;
    material.glyph = '0';
    material.blockingLevel = 1;
    DEBUG_CONSOLE = _debugconsole;
}

// Checks if build is overlapping with anything
bool Build_C::isBuildValid() {
    // used when builds are shimmied
    for (int i=0; i<WallPoints.size(); i++) {
        if (WallPoints[i].first < 0 || WallPoints[i].second < 0 || WallPoints[i].first >= MAP_ROWS || WallPoints[i].second >= MAP_COLS) {
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
    }
    return true;
}

// Initialize a build type depending on the job
bool Build_C::init(Jobs job) {
    switch (job) {
        case FARMER:
            name = "Farm";
            maxBuildsz = {10,10};
            break;
        case BREWER:
            name = "Alehouse";
            maxBuildsz = {10,10};
            break;
        case PRIEST:
            name = "Church";
            maxBuildsz = {10,10};
            break;
        default:
            name = "House";
            maxBuildsz = {10,10};
            break;
    }

    BuildLocation.first = rand() % (MAP_ROWS-maxBuildsz.first-1);
    BuildLocation.second = rand() % (MAP_COLS-maxBuildsz.second-1);
    BuildSize.first = 3 + rand() % maxBuildsz.first;
    BuildSize.second = 3 + rand() % maxBuildsz.second;
    findWallPoints();
    if (!isBuildValid()) {
        bool valid = false;
        for (int m=0; m<max_shimmy.first; m++) {
            for (int r=-1; r<2; r++) {
                for (int c=-1; c<2; c++) {
                    BuildLocation.first += r+m;
                    BuildLocation.second += c+m;
                    findWallPoints();
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
void Build_C::findWallPoints() {
    WallPoints.clear();
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tTrying (%d,%d) w:%d h:%d\n", BuildLocation.first, BuildLocation.second, BuildSize.first, BuildSize.second);
    // top row
    for (int i=0; i<BuildSize.first; i++) {
        std::pair<int,int> top;
        top.first = BuildLocation.first + 0;
        top.second = BuildLocation.second + i;
        WallPoints.push_back(top);
    }
    // bottom row
    for (int i=0; i<BuildSize.first; i++) {
        std::pair<int,int> bot;
        bot.first = BuildLocation.first + BuildSize.second - 1 + 0;
        bot.second = BuildLocation.second + i;
        WallPoints.push_back(bot);
    }
    // left col
    for (int i=1; i<BuildSize.second-1; i++) {
        std::pair<int,int> left;
        left.first = BuildLocation.first + i;
        left.second = BuildLocation.second + 0;
        WallPoints.push_back(left);
    }            
    // right col
    for (int i=1; i<BuildSize.second-1; i++) {
        std::pair<int,int> right;
        right.first = BuildLocation.first + i;
        right.second = BuildLocation.second + BuildSize.first - 1 + 0;
        WallPoints.push_back(right);
    }

    // find floor points
    for (int i=BuildLocation.first+1; i<BuildSize.first-1; i++) {
        for (int j=BuildLocation.second+1; j<BuildSize.second-1; j++) {
            FloorPoints.push_back(std::make_pair(i,j));
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
                if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[build c]\tconstruct array @ (%d,%d) = %d\n", WallPoints[0].first, WallPoints[0].second, (*(self->currConstructArray))[WallPoints[0].first][WallPoints[0].second]);
                WallPoints.erase(WallPoints.begin());
                return true;
            } else {
                // being is not at point - moveto it
                self->moveto.first = WallPoints[0].first;
                self->moveto.second = WallPoints[0].second;
                return true;
            }
        } else {
            // failure to finish the room - offscreen error
            needtobuild = false;
            return false;
        }
    }
    // finished build
    needtobuild = false;
    room.Finish(DEBUG_CONSOLE);
    return false; 
}