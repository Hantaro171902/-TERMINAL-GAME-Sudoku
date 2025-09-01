#pragma once
#include <string>
#include "board.hpp"

class Terminal {
public:
    explicit (Board* g, const char* navKeys);
    ~Terminal();

    void printBoard();
    void moveCursor(int row, int col);
    void changeMode(const std::string& s);
    void check(); // toggle check colors
    void select(int val); // highlight number

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
    std::string mode;
    bool checkColors = false;
    int cursorRow = 0;
    int cursorCol = 0;
    int highlightNum = 0; // 0 = none

    // layout
    int boardTop = 2; // y
    int boardLeft = 4; // x
    int BoardRows = 19; // 9 cells * 2 + 1 border lines
    int BoardCols = 37; // 9 cells * 4 + 1 border columns

    // controls
    char leftKey;
    char downKey;
    char upKey;
    char rightKey;
};