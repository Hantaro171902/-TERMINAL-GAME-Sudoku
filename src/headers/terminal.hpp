#pragma once
#include <string>
#include "board.hpp"
#include <chrono>

class Terminal {
public:
    explicit Terminal(Board* g, const char* navKeys);
    ~Terminal();

    void printBoard();
    void moveCursor(int row, int col);
    void changeMode(const std::string& s);
    void check(); // toggle check colors
    void select(int val); // highlight number
    void setElapsedSeconds(int seconds); // update timer value shown
    void resetUI(); // reset highlights/check colors/state

private:
    // drawing helpers
    void drawFrame();
    void drawCoords();
    void drawInstructions();
    void drawPencil();
    void drawNumbers();
    void drawMode();
    void drawCursor();

private:
    Board* game;
    int elapsedSeconds = 0; // seconds to display in the timer
    std::string mode;
    bool checkColors = false;
    int cursorRow = 0;
    int cursorCol = 0;
    int highlightNum = 0; // 0 = none

    // layout
    int boardTop = 12; // y - moved down to avoid title/timer overlap
    int boardLeft = 4; // x
    int BoardRows = 19; // 9 cells * 2 + 1 border lines
    int BoardCols = 37; // 9 cells * 4 + 1 border columns

    // controls
    char leftKey;
    char downKey;
    char upKey;
    char rightKey;
};