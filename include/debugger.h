#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "Windows.h"
#include <string>

class Debugger {
    public:
        Debugger();
        LPSTARTUPINFOA si;
        PROCESS_INFORMATION pi;
        void output(std::string cmd);
};

Debugger& getDebugConsole();

#endif