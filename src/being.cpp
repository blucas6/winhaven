#include "being.h"
#include "components.h"

short BEING_ID = 1;

Being::Being(std::string t, char gly, int c, std::pair<int,int> _pos, CConsoleLoggerEx *_debugconsole, std::vector<std::vector<int>> *blocking_array, std::vector<std::vector<int>> *construct_array) {
    ID = BEING_ID++;
    type = t;
    glyph = gly;
    color = c;
    pos = _pos;
    moveto.first = -1;  // so movement component wont move the being
    moveto.second = -1;
    thought_list.push_back(NO_THOT);
    thought_list.push_back(RELAX);
    DEBUG_CONSOLE = _debugconsole;
    laziness.first = 1 + rand() % 5;
    laziness.second = laziness.first;
    currBlockingArray = blocking_array;
    currConstructArray = construct_array; 
}

void Being::NewThought() {
    int c = rand() % thought_list.size();
    thought = thought_list[c];
}

void Being::Update() {
    // currBlockingArray = blocking_array;
    // Update new thought for this being
    if (thought == NO_THOT)  {
        NewThought();
        state = IDLE;
    }
    else if (thought == RELAX) {
        state = WANDER;
        if (laziness.second - 1 <= 0) {
            laziness.second = laziness.first;
            NewThought();
        } else laziness.second--;
    }
    DEBUG_CONSOLE->cprintf("[being]\tUpdating being ID(%d) [T:%d | S:%d | P:%d,%d]\n", ID, thought, state, pos.first, pos.second);
    // Update all the components for this being
    for (Component *comp : ComponentList) {
        comp->Update(this);
    }
}

Human::Human(std::pair<int,int> _pos, Jobs _job, std::vector<Construct*> *buildingListp, CConsoleLoggerEx *_debugconsole, std::vector<std::vector<int>> *blocking_array, std::vector<std::vector<int>> *construct_array) 
: Being("Human", '@', FG_WHITE, _pos, _debugconsole, blocking_array, construct_array) {
    // give a human a job component
    ComponentList.push_back(new Job_C(_job, _debugconsole));
    ComponentList.push_back(new Build_C(buildingListp, _debugconsole));
    if ( ((Build_C*)ComponentList.back())->init(_job) ) {
        thought_list.push_back(BUILD);
    }
    ComponentList.push_back(new Movement_C(&pos, _debugconsole, blockingLevel));
}