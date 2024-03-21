#ifndef BEING_H
#define BEING_H

#include "enums.h"
#include "construct.h"
#include "ConsoleLogger.h"
#include "graphics.h"

class Component;

class Being {
    public:
        Being(std::string t, char gly, int c, std::pair<int,int> _pos, CConsoleLoggerEx *_debugconsole, std::vector<std::vector<int>> *blocking_array);
        short ID;
        std::string type;
        char glyph;
        int color;
        std::pair<int,int> worldsz;                         // used for astar boundaries
        std::pair<int,int> pos;
        std::pair<int,int> moveto;
        int blockingLevel = 1;                              // cannot pass through this blocking level
        States state = IDLE;
        Thoughts thought = NO_THOT;
        std::vector<Thoughts> thought_list;                 // all possible thoughts
        bool newthought = false;                            // dictates whether or not to pick a new thought
        std::vector<Component*> ComponentList;
        CConsoleLoggerEx *DEBUG_CONSOLE;
        std::vector<std::vector<int>> *currBlockingArray;   // pointer to the current turns blocking_array for astar

        std::pair<int,int> laziness;        // how long they relax [MAX, COUNTER]

        virtual void NewThought();
        virtual void Update(std::vector<std::vector<int>> *blocking_array);
};

class Human : public Being {
    public:
        Human(std::pair<int,int> p, Jobs j, std::vector<Construct*> *buildingListp, CConsoleLoggerEx *_debugconsole, std::vector<std::vector<int>> *blocking_array);
};

#endif