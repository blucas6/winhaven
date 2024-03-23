#ifndef ASTAR_H
#define ASTAR_H

#include "math.h"
#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <string>
#include "ConsoleLogger.h"
#include "config.h"

// A structure to hold the necessary parameters
class Cell {
    public:
        // Row and Column index of its parent
        std::pair<int,int> parent;

        // f = g + h - m
        double f, g, h, m;
        std::string mdir;
        Cell() {
            parent.first = -1;
            parent.second = -1;
            f = -1;
            g = -1;
            h = -1;
            m = -1;
            mdir = "none";
        }
};

class Astar {
    public:
        Astar() {
            R = MAP_ROWS;
            C = MAP_COLS;
        }
        Astar(CConsoleLoggerEx *_debugconsole) {
            R = MAP_ROWS;
            C = MAP_COLS;
            DEBUG_CONSOLE = _debugconsole;
        }
        Astar(std::pair<int,int> _worldsz) {
            R = _worldsz.first;
            C = _worldsz.second;
        };
        int C;
        int R;
        int block_level;
        bool useDiag;
        bool useMomentum;
        CConsoleLoggerEx *DEBUG_CONSOLE = nullptr;
        bool isValid(std::pair<int,int> point);
        bool isUnBlocked(std::vector<std::vector<int>> grid, std::pair<int,int> point);
        bool isDestination(std::pair<int,int> position, std::pair<int,int> dest);
        double calculateHValue(std::pair<int,int> src, std::pair<int,int> dest);
        void tracePath(Cell cellDetails[], std::pair<int,int> dest, int C, std::vector<std::pair<int,int>> &Path); 
        bool astar(std::vector<std::vector<int>> grid, std::pair<int,int> src, std::pair<int,int> dest, std::vector<std::pair<int,int>> &Path);
};


#endif