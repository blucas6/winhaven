#ifndef BEING_H
#define BEING_H

#include "enums.h"
#include "construct.h"
#include "ConsoleLogger.h"
#include "graphics.h"
#include "land.h"
#include <memory>
#include <string>

class Town;

class Component;

class Being {
    public:
        Being(std::string t, char gly, int c, std::pair<int,int> _pos, CConsoleLoggerEx *_debugconsole, std::vector<std::vector<int>> *blocking_array, std::vector<std::vector<int>> *construct_array, std::vector<std::vector<std::shared_ptr<PointStruct>>> *pointstruct_array);
        virtual ~Being() {};
        int ID;
        std::string type;
        char glyph;
        int color;
        std::pair<int,int> pos;
        std::pair<int,int> moveto;
        int blockingLevel = 1;                              // cannot pass through this blocking level
        States state = IDLE;
        Thoughts thought = NO_THOT;
        std::vector<Thoughts> thought_list;                 // all possible thoughts
        bool newthought = false;                            // dictates whether or not to pick a new thought
        std::vector<std::shared_ptr<Component>> ComponentList;
        CConsoleLoggerEx *DEBUG_CONSOLE = nullptr;
        std::vector<std::vector<int>> *currBlockingArray = nullptr;     // pointer to the current turns blocking_array for astar
        std::vector<std::vector<int>> *currConstructArray = nullptr;    // pointer to the mapslices construction array - used to update build pts
        std::vector<std::vector<std::shared_ptr<PointStruct>>> *currPTArray = nullptr;           // pointer to the mapslices pointstructs map - used for accessing specific structures by index


        std::pair<int,int> goToPT;  // delete me
        Town *myTown = nullptr;

        std::pair<int,int> laziness;        // how long they relax [MAX, COUNTER]

        std::vector<std::shared_ptr<PointStruct>> Inventory;    // all items being held

        virtual void NewThought();
        virtual void Update();
        void addConstruct(std::shared_ptr<PointStruct> pt);
        void removeConstruct(std::pair<int,int> pt);
        void updateConstruct(std::pair<int,int> pt);
        void clearComponents();
        void goToGOTOPT();
        int howManyInventory();
};

class Human : public Being {
    public:
        Human(std::pair<int,int> _pos, Jobs _job, std::vector<std::shared_ptr<Construct>> *buildingListp, std::vector<std::vector<Land>> *_landPiecesPtr, CConsoleLoggerEx *_debugconsole, std::vector<std::vector<int>> *blocking_array, std::vector<std::vector<int>> *construct_array, std::vector<std::vector<std::shared_ptr<PointStruct>>> *pointstruct_array);
};

#endif