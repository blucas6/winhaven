#include <iostream>
#include <chrono>
#include <cstdlib>
#include "game.h"
#include "config.h"

int main(int argc, char *argv[]) {
    srand((unsigned) time(NULL));
    int turncount;
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <amount_of_turns>" << std::endl;
        std::cout << "\tDefaults to 100" << std::endl;
        turncount = 100;
    } else {
        turncount = atoi(argv[1]);
    }

    auto start = std::chrono::high_resolution_clock::now();

    Game game;

    auto gen = std::chrono::high_resolution_clock::now();

    for (int i=0; i<turncount; i++) {
        game.UpdateWorld();
    }

    auto stop = std::chrono::high_resolution_clock::now();

    auto genduration = std::chrono::duration_cast<std::chrono::milliseconds>(gen - start);
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - gen);
    double timeperloop = duration.count() / static_cast<double>(turncount);

    std::cout << "\n=Results=" << std::endl;
    std::cout << " World Gen:   " << genduration.count() << "ms\tMap Size: [" << MAP_ROWS << "," << MAP_COLS << "]" << std::endl;
    std::cout << " Run Time:    " << duration.count() << "ms" << std::endl;
    std::cout << " TimePerLoop: " << timeperloop << "ms\tFPS:" << 1000/timeperloop <<std::endl;


}