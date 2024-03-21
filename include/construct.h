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
        Construct(std::string _type);
        std::string type;
        std::vector<PointStruct> PointStructs;

        void Update();
};

class Room : public Construct {
    public:
        Room(std::string _name);
        int width;
        int height;
        PointStruct corner;
        PointStruct wall_h;
        PointStruct wall_v;
        PointStruct floor;
        PointStruct roof;
        PointStruct door;

        void Finish(CConsoleLoggerEx *debugconsole);
};


#endif