#include "debugger.h"

Debugger::Debugger() {
    ZeroMemory(&si, sizeof(si));
    si->cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
}

void Debugger::output(std::string cmd) {
    if (CreateProcessA(NULL, const_cast<char *>(cmd.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

Debugger& getDebugConsole() {
    static Debugger instance; // Initialized when the function is first called
    return instance;
}