#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <string>
#include "graphics.h"
#include "ConsoleLogger.h"
#include "enums.h"
#include "config.h"
#include <memory>

class PointStruct {
    public:
        PointStruct();
        PointStruct(CConsoleLoggerEx *_debugconsole);
        PointStruct(std::string name, char glyph, int color, int blockingLevel, PT_Type type, std::pair<int,int> pos);
        virtual ~PointStruct() {};
        CConsoleLoggerEx *DEBUG_CONSOLE = nullptr;
        std::pair<int,int> pos;
        std::string name;
        PT_Type type;
        char glyph;
        int color;
        int blockingLevel;

        virtual void Use();
        virtual void Open();
        virtual void Close();
};

class Material : public PointStruct {
    public:
        Material(std::pair<int,int> pos);
};

class CornerTL : public PointStruct {
    public:
        CornerTL(std::pair<int,int> pos);
};

class CornerTR : public PointStruct {
    public:
        CornerTR(std::pair<int,int> pos);
};

class CornerBL : public PointStruct {
    public:
        CornerBL(std::pair<int,int> pos);
};

class CornerBR : public PointStruct {
    public:
        CornerBR(std::pair<int,int> pos);
};

class Wall_H : public PointStruct {
    public:
        Wall_H(std::pair<int,int> pos);
};

class Wall_V : public PointStruct {
    public:
        Wall_V(std::pair<int,int> pos);
};

class Floor : public PointStruct {
    public:
        Floor(std::pair<int,int> pos);
};

class Door_H : public PointStruct {
    public:
        Door_H(std::pair<int,int> pos);
        char open_glyph;
        char close_glyph;
        void Open();
        void Close();
};

class Door_V : public PointStruct {
    public:
        Door_V(std::pair<int,int> pos);
        char open_glyph;
        char close_glyph;
        void Open();
        void Close();
};

class Table : public PointStruct {
    public:
        Table(std::pair<int,int> pos);
};

class Chair : public PointStruct {
    public:
        Chair(std::pair<int,int> pos);
};

class Construct {
    public:
        Construct(std::string _type, CConsoleLoggerEx *_debugconsole);
        std::string type;
        CConsoleLoggerEx *DEBUG_CONSOLE = nullptr;
        std::vector<std::vector<std::shared_ptr<PointStruct>>> *currPointStruct_Array = nullptr;    // pointer to actual pointstruct array
        std::vector<std::vector<int>> *currConstructArray = nullptr;    // pointer to the construct array
        std::vector<std::pair<int,int>> wallPoints;     // pts that show where the structure is planned to be - needed for checking validity
        std::vector<std::pair<int,int>> floorPoints;    // inside area of build 
        std::vector<std::pair<int,int>> gardenPoints;   // if build has a garden
        
        void addConstruct(std::shared_ptr<PointStruct> pt, std::pair<int,int> pos);
        void Update();
};

class Room : public Construct {
    public:
        Room(std::string _name, CConsoleLoggerEx *_debugconsole);
        int width;
        int height;

        bool Finish(bool isFence=false);
        void MakeWalls();
        std::vector<std::shared_ptr<PointStruct>> AddFlooring();
        std::vector<std::shared_ptr<PointStruct>> AddFurniture(Furniture type, std::vector<std::shared_ptr<PointStruct>> flooring, int pt);
        int ValidFloorPt(std::vector<std::shared_ptr<PointStruct>> flooring, std::pair<int,int> pt);
};


#endif