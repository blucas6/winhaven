#include "being.h"
#include "components.h"

short BEING_ID = 1;

Being::Being(std::string t, char gly, int c, std::pair<int,int> _pos, CConsoleLoggerEx *_debugconsole, std::vector<std::vector<int>> *blocking_array, std::vector<std::vector<int>> *construct_array, std::vector<std::vector<std::shared_ptr<PointStruct>>> *pointstruct_array) {
    ID = BEING_ID++;
    type = t;
    glyph = gly;
    color = c;
    pos = _pos;
    moveto.first = -1;  // so movement component wont move the being
    moveto.second = -1;
    thought_list.push_back(NO_THOT);
    thought_list.push_back(RELAX_THOT);
    DEBUG_CONSOLE = _debugconsole;
    laziness.first = 1 + rand() % 5;
    laziness.second = laziness.first;
    currBlockingArray = blocking_array;
    currConstructArray = construct_array; 
    currPTArray = pointstruct_array;
}

void Being::goToGOTOPT() {
    thought = GOTO;
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
    else if (thought == RELAX_THOT) {
        state = WANDER;
        if (laziness.second - 1 <= 0) {
            laziness.second = laziness.first;
            NewThought();
        } else laziness.second--;
    } else if (thought == GOTO) {
        moveto = goToPT;
    }
    if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[being]\tUpdating being ID(%d) [T:%d | S:%d | P:%d,%d]\n", ID, thought, state, pos.first, pos.second);
    // Update all the components for this being
    for (std::shared_ptr<Component> comp : ComponentList) {
        comp->Update(this);
    }
}

void Being::addConstruct(std::shared_ptr<PointStruct> pt) {
    if (pt->pos.first > -1 && pt->pos.second > -1) {
        (*currConstructArray)[pt->pos.first][pt->pos.second] = pt->blockingLevel;
        (*currPTArray)[pt->pos.first][pt->pos.second] = pt;
        if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[being]\tAdding Structure (%d)\n", (*currPTArray)[pt->pos.first][pt->pos.second]->type);
    } else if (DEBUG_CONSOLE != nullptr) DEBUG_CONSOLE->cprintf("[being]\t**Error** Adding PT with no position!\n");
}

void Being::removeConstruct(std::pair<int,int> pt) {
    (*currConstructArray)[pt.first][pt.second] = 0;
    (*currPTArray)[pt.first][pt.second] = nullptr;
}

void Being::clearComponents() {
    ComponentList.clear();
}

Human::Human(std::pair<int,int> _pos, Jobs _job, std::vector<std::shared_ptr<Construct>> *buildingListp, std::vector<std::vector<Land>> *_landPiecesPtr, CConsoleLoggerEx *_debugconsole, std::vector<std::vector<int>> *blocking_array, std::vector<std::vector<int>> *construct_array, std::vector<std::vector<std::shared_ptr<PointStruct>>> *pointstruct_array) 
: Being("Human", '@', FG_WHITE, _pos, _debugconsole, blocking_array, construct_array, pointstruct_array) {
    // give a human a job component
    switch(_job) {
        case FARMER:
            ComponentList.push_back(std::make_shared<Job_C_Farmer>(_debugconsole));
            break;
        case BREWER:
            break;
        case PRIEST:
            break;
        default:
            break;
    }
    ComponentList.push_back(std::make_shared<Build_C>(buildingListp, _landPiecesPtr, _debugconsole));
    if ( std::dynamic_pointer_cast<Build_C>(ComponentList.back())->init(this,_job) ) {
        thought_list.push_back(BUILD_THOT);
    }
    ComponentList.push_back(std::make_shared<Movement_C>(&pos, _debugconsole, blockingLevel));
}