#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include "graphics.h"
#include "ConsoleLogger.h"
#include "enums.h"

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
        std::vector<PointStruct> PointStructs;          // actual structures of the build
        std::vector<std::pair<int,int>> wallPoints;     // pts that show where the structure is planned to be - needed for checking validity
        std::vector<std::pair<int,int>> floorPoints;    // inside area of build 
        std::vector<std::pair<int,int>> gardenPoints;   // if build has a garden

        void Update();
};

class Room : public Construct {
    public:
        Room(std::string _name, CConsoleLoggerEx *_debugconsole);
        int width;
        int height;
        PointStruct corner_tl;
        PointStruct corner_tr;
        PointStruct corner_bl;
        PointStruct corner_br;
        
        PointStruct wall_h;
        PointStruct wall_v;
        PointStruct floor;
        PointStruct roof;
        PointStruct door;
        PointStruct fence;
        PointStruct table;
        PointStruct chair;

        void Finish(bool isFence=false);
        void MakeWalls();
        std::vector<PointStruct> AddFlooring();
        std::vector<PointStruct> AddFurniture(Furniture type, std::vector<PointStruct> flooring, int pt);
        int ValidFloorPt(std::vector<PointStruct> flooring, std::pair<int,int> pt);
};


#endif