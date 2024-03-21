#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <Tchar.h>

#define COLS 120
#define ROWS 30

HANDLE wHnd;    // Handle to write to the console.
HANDLE rHnd;    // Handle to read from the console.

int _tmain(int argc, _TCHAR* argv[]) {

    // Set up the handles for reading/writing:
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);

    // Change the window title:
    SetConsoleTitle(TEXT("Win32 Console Control Demo"));

    // Set up the required window size:
    SMALL_RECT windowSize = {0, 0, COLS-1, ROWS-1};
    
    // Change the console window size:
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
    
    // Create a COORD to hold the buffer size:
    COORD bufferSize = {COLS, ROWS};

    // Change the internal buffer size:
    SetConsoleScreenBufferSize(wHnd, bufferSize);

    CHAR_INFO consoleBuffer[COLS * ROWS];
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
        
            // An ANSI character is in the range 0-255,
            // so use % to keep it in this range.
            consoleBuffer[x + COLS * y].Char.AsciiChar = rand() % 256;

            // The colour is also in the range 0-255,
            // as it is a pair of 4-bit colours.
            consoleBuffer[x + COLS * y].Attributes = rand() % 256;
        }
    }

    // Set up the positions:
    COORD charBufSize = {COLS,ROWS};
    COORD characterPos = {0,0};
    SMALL_RECT writeArea = {0,0,COLS-1,ROWS-1}; 

    // Write the characters:
    WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);

    // Move the cursor down a row so we can see the character:
    printf("\n");

}