
#include "astar.h"
#include <iostream>

void init(char map[], int grid[], int C, int R) {
    for(int i=0; i<R; i++) {
        for(int j=0; j<C; j++) {
            map[i * C + j] = '.';
            grid[i * C + j] = 0;
        }
    }
}

void placeWall(std::vector<std::pair<int,int>> walls, int grid[], char map[], int C) {
    for(const auto& el : walls) {
        grid[el.first * C + el.second] = 1;
        map[el.first * C + el.second] = 'O';
    }
}

void printmap(char map[], std::vector<std::pair<int,int>> &Path, std::pair<int, int> src, std::pair<int,int> dest, int C, int R) {
    std::cout << "The Path is: " << std::endl;
    map[src.first * C + src.second] = 'S';
    map[src.first * C + src.second] = 'D';
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
    srand((unsigned) time(NULL));
    const int R = 10;
    const int C = 20;
    int wallpts = 50;
    int grid[R * C];
    char map[R * C];
    std::vector<std::pair<int,int>> Path;
    std::vector<std::pair<int,int>> walls;
    std::pair<int,int> src(0, 0);
    std::pair<int,int> dest(R-1, C-1);
    // std::pair<int,int> pt;
    // pt.first = 4;
    // pt.second = 4;
    // walls.push_back(pt);
    // pt.first = 5;
    // walls.push_back(pt);
    // pt.first = 6;
    // walls.push_back(pt);
    // pt.first = 3;
    // walls.push_back(pt);

    for (int i=0; i<wallpts; i++) {    
        std::pair<int,int> pt;
        pt.first = rand() % (R-1);
        pt.second = rand() % (C-1);
        if ((pt.first == src.first && pt.second == src.second) || (pt.first == dest.first && pt.second == dest.second)) break;
        walls.push_back(pt);
    }
    init(map, grid, C, R);
    placeWall(walls, grid, map, C);
    printmap(map, Path, src, dest, C, R);
    Astar astar;
    if (!astar.astar(grid, src, dest, C, R, 1, Path, true, false)) std::cout << "No path found!" << std::endl;
    printmap(map, Path, src, dest, C, R);
    return 0;
}