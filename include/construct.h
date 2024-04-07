#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include "graphics.h"
#include "ConsoleLogger.h"
#include "enums.h"
#include <string>

class PointStruct {
    public:
        PointStruct();
        PointStruct(CConsoleLoggerEx *_debugconsole);
        virtual ~PointStruct() {};
        CConsoleLoggerEx *DEBUG_CONSOLE = nullptr;
        std::pair<int,int> pos;
        std::string name;
        char glyph;
        int color;
        int blockingLevel;

        virtual void Use();
        virtual void Open();
        virtual void Close();
};

class Material : public PointStruct {
    public:
        Material();
};

class CornerTL : public PointStruct {
    public:
        CornerTL();
};

class CornerTR : public PointStruct {
    public:
        CornerTR();
};

class CornerBL : public PointStruct {
    public:
        CornerBL();
};

class CornerBR : public PointStruct {
    public:
        CornerBR();
};

class Wall_H : public PointStruct {
    public:
        Wall_H();
};

class Wall_V : public PointStruct {
    public:
        Wall_V();
};

class Floor : public PointStruct {
    public:
        Floor();
};

class Door_H : public PointStruct {
    public:
        Door_H();
        Door_H(std::pair<int,int> pos);
        char open_glyph;
        char close_glyph;
        void Open();
        void Close();
};

class Door_V : public PointStruct {
    public:
        Door_V();
        Door_V(std::pair<int,int> pos);
        char open_glyph;
        char close_glyph;
        void Open();
        void Close();
};

class Table : public PointStruct {
    public:
        Table();
};

class Chair : public PointStruct {
    public:
        Chair();
};

class Construct {
    public:
        Construct(std::string _type, CConsoleLoggerEx *_debugconsole);
        std::string type;
        CConsoleLoggerEx *DEBUG_CONSOLE = nullptr;
        std::vector<PointStruct*> Structures;          // actual structures of the build
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

        std::vector<std::vector<int>> *currConstructArray = nullptr;   // pointer to the mapslices construction array - used to mark door pt as special for astar

        void Finish(bool isFence=false);
        void MakeWalls();
        std::vector<PointStruct*> AddFlooring();
        std::vector<PointStruct*> AddFurniture(Furniture type, std::vector<PointStruct*> flooring, int pt);
        int ValidFloorPt(std::vector<PointStruct*> flooring, std::pair<int,int> pt);
};


#endif