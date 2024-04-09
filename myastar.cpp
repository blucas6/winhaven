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

bool isValid(std::pair<int,int> point, int C, int R) { 
    // Returns true if row number and column number is in range
    if(point.first >=0 && point.first < R && point.second >= 0 && point.second < C) {
        return true;
    }
    return false;
}

bool isUnBlocked(int grid[], std::pair<int,int> point, int C, int R, int block_level) {
    // Returns true if the cell is not blocked else false
    // block_level: highest level of terrain difficulty that is considered "blocking"
    return isValid(point, C, R) && (grid[point.first * C + point.second] < block_level);
}

bool isDestination(std::pair<int,int> position, std::pair<int,int> dest) {
    return position == dest;
}

double calculateHValue(std::pair<int,int> src, std::pair<int,int> dest) {
    // h is estimated with the two points distance formula
    return sqrt(pow((src.first - dest.first), 2.0) + pow((src.second - dest.second), 2.0));
}

void tracePath(Cell cellDetails[], std::pair<int,int> dest, int C, std::vector<std::pair<int,int>> &Path) { 
    int row = dest.first;
    int col = dest.second;
    Path.emplace_back(row, col);
    std::pair<int,int> next_node = cellDetails[row * C + col].parent;
    row = next_node.first;
    col = next_node.second;
    do {
        Path.push_back(next_node);
        next_node = cellDetails[row * C + col].parent;
        row = next_node.first;
        col = next_node.second;
    } while (cellDetails[row * C + col].parent != next_node);
    
    Path.emplace_back(row, col);
}

void astar(int grid[], std::pair<int,int> src, std::pair<int,int> dest, int C, int R, int block_level, std::vector<std::pair<int,int>> &Path, bool useDiag, bool useMomentum) {
    std::cout << "Find Path from (" << src.first << "," << src.second << ") to (" << dest.first << "," << dest.second << ")" << std::endl;
    // If the source is out of range
    if (!isValid(src, C, R)) {
        printf("Source is invalid\n");
        return;
    }
 
    // If the destination is out of range
    if (!isValid(dest, C, R)) {
        printf("Destination is invalid\n");
        return;
    }
 
    // Either the source or the destination is blocked
    if (!isUnBlocked(grid, src, C, R, block_level) || !isUnBlocked(grid, dest, C, R, block_level)) {
        printf("Source or the destination is blocked\n");
        return;
    }
 
    // If the destination cell is the same as source cell
    if (isDestination(src, dest)) {
        printf("We are already at the destination\n");
        return;
    }
 
    // Create a closed list and initialise it to false which
    // means that no cell has been included yet This closed
    // list is implemented as a boolean 2D array
    bool *closedList = new bool[C * R];
    if (closedList == nullptr) std::cout << "closed list ptr failed!" << std::endl;
    // memset(closedList, false, sizeof(closedList));
    for (int i=0; i < R; i++) {
        for (int j=0; j<C; j++) {
            closedList[i * C + j] = false;
        }
    }
 
    // Declare a 2D array of structure to hold the details
    // of that cell
    Cell *cellDetails = new Cell[C * R];
    if (cellDetails == nullptr) std::cout << "celldetails ptr failed!" << std::endl;
    int i, j;
    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    cellDetails[i * C + j].f = 0.0;
    cellDetails[i * C + j].g = 0.0;
    cellDetails[i * C + j].h = 0.0;
    cellDetails[i * C + j].m = 0.0;
    cellDetails[i * C + j].parent = { i, j };
 
    /*
    Create an open list having information as-
    <f, <i, j>>
    where f = g + h,
    and i, j are the row and column index of that cell
    Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    This open list is implemented as a set of std::tuple.*/
    std::priority_queue<std::tuple<double,int,int>, std::vector<std::tuple<double,int,int>>, std::greater<std::tuple<double,int,int>>> openList;
 
    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.emplace(0.0, i, j);
 
    // We set this boolean value as false as initially
    // the destination is not reached.
    while (!openList.empty()) {
        const std::tuple<double,int,int>& p = openList.top();
        // Add this vertex to the closed list
        i = std::get<1>(p); // second element of tuple
        j = std::get<2>(p); // third element of tuple
 
        // Remove this vertex from the open list
        openList.pop();
        closedList[i * C + j] = true;
        // std::cout << "CELL: (" << i << "," << j << ") " << closedList[i * C + j] << std::endl;
        // for (int i=0; i < R; i++) {
        //     for (int j=0; j<C; j++) {
        //         std::cout << "(" << cellDetails[i * C + j].parent.first << "," << cellDetails[i * C + j].parent.second << ")";
        //     }
        //     std::cout << std::endl;
        // }
        /*
                Generating all the 8 successor of this cell
                        N.W N N.E
                        \ | /
                        \ | /
                        W----Cell----E
                        / | \
                        / | \
                        S.W S S.E
 
                Cell-->Popped Cell (i, j)
                N --> North     (i-1, j)
                S --> South     (i+1, j)
                E --> East     (i, j+1)
                W --> West         (i, j-1)
                N.E--> North-East (i-1, j+1)
                N.W--> North-West (i-1, j-1)
                S.E--> South-East (i+1, j+1)
                S.W--> South-West (i+1, j-1)
        */
        for (int add_x = -1; add_x <= 1; add_x++) {
            for (int add_y = -1; add_y <= 1; add_y++) {
                if ((useDiag) || ((add_x==0 && add_y==-1) || (add_x==1 && add_y==0) || (add_x==0 && add_y==1) || (add_x==-1 && add_y==0)) ) {
                    std::pair<int,int> neighbour(i + add_x, j + add_y);
                    // Only process this cell if this is a valid
                    if (isValid(neighbour, C, R)) {
                        //std::cout << "Neighbour ("<< neighbour.first << "," << neighbour.second << ") VALID " << std::endl;
                        // If the destination cell is the same
                        // as the current successor
                        if (isDestination(neighbour, dest)) { // Set the Parent of the destination cell
                            cellDetails[neighbour.first * C + neighbour.second].parent = { i, j };
                            std::cout << "Cell Details List: " << std::endl;
                            for(int i=0; i<R; i++) {
                                for(int j=0; j<C; j++) {
                                    printf("(%2.4f)", cellDetails[i * C + j].f);
                                }
                                std::cout << std::endl;
                            }
                            // printf("The destination cell is found\n");
                            tracePath(cellDetails, dest, C, Path);
                            delete[] cellDetails;
                            delete[] closedList;
                            return;
                        }
                        // If the successor is already on the
                        // closed list or if it is blocked, then
                        // ignore it.  Else do the following
                        else if (!closedList[neighbour.first * C + neighbour.second] && isUnBlocked(grid, neighbour, C, R, block_level)) {
                            double gNew, hNew, fNew, mNew;
                            std::string mdirNew;
                            gNew = cellDetails[i * C + j].g + 1.0;
                            hNew = calculateHValue(neighbour, dest);
                            // find which direction
                            if(add_x==0 && add_y==-1) {         // up
                                mdirNew = "top"; 
                            } else if(add_x==0 && add_y==1) {   // down
                                mdirNew = "down";
                            } else if(add_x==1 && add_y==0) {   // right
                                mdirNew = "right";
                            } else if(add_x==-1 && add_y==0) {  // left
                                mdirNew = "left";
                            }
                            if(useMomentum && cellDetails[i * C + j].mdir == mdirNew) {
                                mNew = cellDetails[i * C + j].m + 1.0;
                            } else {
                                mNew = 0.0;
                            }
                            fNew = gNew + hNew - mNew;
    
                            // If it isn’t on the open list, add
                            // it to the open list. Make the
                            // current square the parent of this
                            // square. Record the f, g, and h
                            // costs of the square cell
                            //             OR
                            // If it is on the open list
                            // already, check to see if this
                            // path to that square is better,
                            // using 'f' cost as the measure.
                            if (cellDetails[neighbour.first * C + neighbour.second].f == -1 || cellDetails[neighbour.first* C +neighbour.second].f > fNew) {
                                openList.emplace(fNew, neighbour.first, neighbour.second);
                                // Update the details of this cell
                                cellDetails[neighbour.first* C +neighbour.second].g = gNew;
                                cellDetails[neighbour.first* C +neighbour.second].h = hNew;
                                cellDetails[neighbour.first* C +neighbour.second].f = fNew;
                                cellDetails[neighbour.first* C +neighbour.second].m = mNew;
                                cellDetails[neighbour.first* C +neighbour.second].mdir = mdirNew;
                                cellDetails[neighbour.first* C +neighbour.second].parent = { i, j };
                            }
                        }
                    }
                }
            }
        }
    }
 
    // When the destination cell is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destination cell. This may happen when the
    // there is no way to destination cell (due to
    // blockages)
    printf("Failed to find the Destination Cell\n");
}

void placeWall(std::vector<std::pair<int,int>> walls, int grid[], char map[], int C) {
    for(const auto& el : walls) {
        grid[el.first * C + el.second] = 1;
        map[el.first * C + el.second] = 'O';
    }
}

void init(char map[], int grid[], int C, int R) {
    for(int i=0; i<R; i++) {
        for(int j=0; j<C; j++) {
            map[i * C + j] = '.';
            grid[i * C + j] = 0;
        }
    }
}

void printmap(char map[], std::vector<std::pair<int,int>> &Path, std::pair<int, int> src, std::pair<int,int> dest, int C, int R) {
    std::cout << "The Path is: " << std::endl;
    for (const auto& el : Path) {
        map[el.first * C + el.second] = 'x';
        std::cout << "("<< el.first << "," << el.second << ") ";
    }
    std::cout << std::endl;
    for (int i=0; i<R; i++) {
        for (int j=0; j<C; j++) {
            std::cout << map[i * C + j];
        }
        std::cout << std::endl;
    }
}

int main() {
    const int C = 15;
    const int R = 10;
    int grid[R * C];
    char map[R * C];
    init(map, grid, C, R);
    std::pair<int,int> src(0, 0);
    std::pair<int,int> dest(R-1, C-1);
    std::vector<std::pair<int,int>> walls;
    std::vector<std::pair<int,int>> Path;
    
    walls.push_back(std::make_pair(5,2));
    walls.push_back(std::make_pair(5,3));
    walls.push_back(std::make_pair(5,4));
    walls.push_back(std::make_pair(5,5));
    walls.push_back(std::make_pair(5,6));
    walls.push_back(std::make_pair(5,8));
    walls.push_back(std::make_pair(5,0));
    walls.push_back(std::make_pair(5,7));
    walls.push_back(std::make_pair(8,9));



    placeWall(walls, grid, map, C);
    for (int i=0; i<R; i++) {
        for (int j=0; j<C; j++) {
            std::cout << grid[ i * C + j];
        }
        std::cout << std::endl;
    }
    map[src.first + C * src.second] = 'S';
    map[dest.first + C * dest.second] = 'D';
    std::cout << "Start: " << std::endl;
    printmap(map, Path, src, dest, C, R);

    astar(grid, src, dest, C, R, 1, Path, true, false);

    std::cout << "Finish: " << std::endl;
    printmap(map, Path, src, dest, C, R);

    return 0;
}