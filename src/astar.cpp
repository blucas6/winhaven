#include "astar.h"

bool Astar::isValid(std::pair<int,int> point) { 
    // Returns true if row number and column number is in range
    if(point.first >=0 && point.first < R && point.second >= 0 && point.second < C) {
        return true;
    }
    return false;
}

bool Astar::isUnBlocked(std::vector<std::vector<int>> grid , std::pair<int,int> point) {
    // Returns true if the cell is not blocked else false
    // block_level: highest level of terrain difficulty that is considered "blocking"
    return isValid(point) && (grid[point.first][point.second] < block_level);
}

bool Astar::isDestination(std::pair<int,int> position, std::pair<int,int> dest) {
    return position == dest;
}

double Astar::calculateHValue(std::pair<int,int> src, std::pair<int,int> dest) {
    // h is estimated with the two points distance formula
    return sqrt((src.first - dest.first)*(src.first - dest.first)) + ((src.second - dest.second)*(src.second - dest.second));
}

void Astar::tracePath(Cell cellDetails[], std::pair<int,int> dest, int C, std::vector<std::pair<int,int>> &Path) { 
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

bool Astar::astar(std::vector<std::vector<int>> grid, std::pair<int,int> src, std::pair<int,int> dest, std::vector<std::pair<int,int>> &Path, CConsoleLoggerEx *_debugconsole) {
    _debugconsole->cprintf("[astar]\tStarting Astar...");
    // If the source is out of range
    if (!isValid(src)) {
        _debugconsole->cprintf("[astar]\tSource OOR!");
        return false;
    }
 
    // If the destination is out of range
    if (!isValid(dest)) {
        _debugconsole->cprintf("[astar]\tDest OOR!");
        return false;
    }
 
    // Destination is blocked
    if (!isUnBlocked(grid, dest)) {
        _debugconsole->cprintf("[astar]\tDest blocked!");
        return false;
    }
 
    // If the destination cell is the same as source cell
    if (isDestination(src, dest)) {
        _debugconsole->cprintf("[astar]\tAlready at dest!");
        return false;
    }
 
    // Create a closed list and initialise it to false which
    // means that no cell has been included yet This closed
    // list is implemented as a boolean 2D array
    bool *closedList = new bool[C * R];
    // memset(closedList, false, sizeof(closedList));
    for (int i=0; i < R; i++) {
        for (int j=0; j<C; j++) {
            closedList[i * C + j] = false;
        }
    }
 
    // Declare a 2D array of structure to hold the details
    // of that cell
    Cell *cellDetails = new Cell[C * R];
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
                    if (isValid(neighbour)) {
                        // If the destination cell is the same
                        // as the current successor
                        if (isDestination(neighbour, dest)) { // Set the Parent of the destination cell
                            cellDetails[neighbour.first * C + neighbour.second].parent = { i, j };
                            tracePath(cellDetails, dest, C, Path);
                            return true;
                        }
                        // If the successor is already on the
                        // closed list or if it is blocked, then
                        // ignore it.  Else do the following
                        else if (!closedList[neighbour.first * C + neighbour.second] && isUnBlocked(grid, neighbour)) {
                            double gNew, hNew, fNew, mNew;
                            std::string mdirNew;
                            std::pair<int,int> curr;
                            curr.first = i;
                            curr.second = j;
                            gNew = cellDetails[i * C + j].g + 1.0;
                            hNew = calculateHValue(neighbour, dest);
                            // find which direction
                            if (useMomentum) {
                                if(add_x==0 && add_y==-1) {         // up
                                    mdirNew = "top"; 
                                } else if(add_x==0 && add_y==1) {   // down
                                    mdirNew = "down";
                                } else if(add_x==1 && add_y==0) {   // right
                                    mdirNew = "right";
                                } else if(add_x==-1 && add_y==0) {  // left
                                    mdirNew = "left";
                                }
                                if(cellDetails[i*C+j].mdir == mdirNew) { mNew = cellDetails[i * C + j].m + 1.0; }
                                else { mNew = 0.0; }
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
                                // std::cout << "PT (" << neighbour.first << "," << neighbour.second << ")\n\tG:" << gNew << "\n\tH:" << hNew << "\n\tParent:[" << i << "," << j << "]" << std::endl; 
                                cellDetails[neighbour.first* C +neighbour.second].g = gNew;
                                cellDetails[neighbour.first* C +neighbour.second].h = hNew;
                                cellDetails[neighbour.first* C +neighbour.second].f = fNew;
                                cellDetails[neighbour.first* C +neighbour.second].m = mNew;
                                cellDetails[neighbour.first* C +neighbour.second].mdir = mdirNew;
                                cellDetails[neighbour.first* C +neighbour.second].parent = { i, j };
                                // for(int y=0; y<C; y++) {
                                //     for(int x=0; x<R; x++) {
                                //         std::cout << "(" << cellDetails[x * C + y].h << "(" << x << "," << y << ")[" << cellDetails[x * C + y].parent.first << "," << cellDetails[x * C + y].parent.second << "]) ";
                                //     }
                                //     std::cout << std::endl;
                                // }
                                // std::cout << "-----------------------------------------------------------------" << std::endl;
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
    _debugconsole->cprintf("[astar]\tNO PATH");
    return false;
}