#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include "graphics.h"
#include "ConsoleLogger.h"

struct PointStruct {
    std::pair<int,int> pos;
    char glyph;
    int color;
    int blockingLevel;
};

class Construct {
    public:
        Construct(std::string _type, CConsoleLoggerEx *_debugconsole);
        std::string type;
        CConsoleLoggerEx *DEBUG_CONSOLE = nullptr;
        std::vector<PointStruct> PointStructs;
        std::vector<std::pair<int,int>> wallPoints;    // pts that show where the structure is planned to be - needed for checking validity
        std::vector<std::pair<int,int>> floorPoints;   // inside area of build 
        std::vector<std::pair<int,int>> gardenPoints;  // if build has a garden

        void Update();
};

class Room : public Construct {
    public:
        Room(std::string _name, CConsoleLoggerEx *_debugconsole);
        int width;
        int height;
        PointStruct corner;
        PointStruct wall_h;
        PointStruct wall_v;
        PointStruct floor;
        PointStruct roof;
        PointStruct door;
        PointStruct fence;
        PointStruct table;

        void Finish(bool isFence=false);
};


#endif