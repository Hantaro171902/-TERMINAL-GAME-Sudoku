#include "terminal.hpp"
#include "utils.hpp"
#include "color.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

Terminal::Terminal(Board* g, const char* navKeys) : game(g) {
    mode = "Insert mode";
    leftKey = navKeys[0];
    mode = "Insert mode";
    leftKey = navKeys[0];
    downKey = navKeys[1];
    upKey = navKeys[2];
    rightKey = navKeys[3];
    hideCursor();
}

Terminal::~Terminal() {
    showCursor();
    resetTextColor();
}

void Terminal::printBoard() {
    clearTerminal(); // Use full terminal reset
   
    drawFrame();
    drawCoords();
    drawInstructions();
    drawNumbers();
    drawPencil();
    drawMode();
    drawCursor();
    // place caret in a safe line below the board to avoid any accidental echo near the grid
    move_cursor(boardLeft, boardTop + BoardRows + 3);
    cout.flush();
}

void Terminal::drawFrame() {
    // Outer border using double line symbols from utils
    int y = boardTop; 
    int x = boardLeft;

    setTextColor(YELLOW);
    cout << 
R"(                  ______     __  __     _____     ______     __  __     __  __    
                 /\  ___\   /\ \/\ \   /\  __-.  /\  __ \   /\ \/ /    /\ \/\ \   
                 \ \___  \  \ \ \_\ \  \ \ \/\ \ \ \ \/\ \  \ \  _"-.  \ \ \_\ \  
                  \/\_____\  \ \_____\  \ \____-  \ \_____\  \ \_\ \_\  \ \_____\ 
                   \/_____/   \/_____/   \/____/   \/_____/   \/_/\/_/   \/_____/                                                     
    )" << endl;
    resetTextColor();

    // Timer (display accumulated elapsed seconds provided by Game)
    move_cursor(4, 9);
    cout << " [ TIME: " << formatTime(elapsedSeconds) << " ]" << endl;
    move_cursor(x, y);
    cout << SYMBOL_DOUBLE_TOP_LEFT;

    for (int i = 0; i < 9; ++i) {
        cout << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL;
        if (i == 8) cout << SYMBOL_DOUBLE_TOP_RIGHT;
        else if ((i+1) % 3 == 0) cout << SYMBOL_DOUBLE_T_TOP;   // ╦
        else cout << "\u2564";                              // ┬
    }

    // middle rows
    for (int r = 0; r < 9; ++r) {
        // content row
        move_cursor(x, ++y);
        for (int c = 0; c < 28; ++c) {
            if (c % 3 == 0) cout << SYMBOL_DOUBLE_VERTICAL; 
            else cout << SYMBOL_VERTICAL;
            if (c < 9) cout << " ";
        }

        // separator row
        move_cursor(x, ++y);
        if (r == 8) break; // handled by bottom
        if ((r+1) % 3 == 0) {
            cout << "\u2560"; // ╠
            for (int c = 0; c < 9; ++c) {
                cout << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL;
                if (c == 8) cout << "\u2563"; // ╣
                else if ((c+1) % 3 == 0) cout << "\u256C"; // ╬
                else cout << "\u256A"; // ╪
            }
        } else {
            cout << "\u255F"; // ╟
            for (int c = 0; c < 9; ++c) {
                cout << SYMBOL_HORIZONTAL << SYMBOL_HORIZONTAL << SYMBOL_HORIZONTAL;
                if (c == 8) cout << "\u2562"; // ╢
                else if ((c+1) % 3 == 0) cout << "\u256B"; // ╫
                else cout << SYMBOL_INTERSECT; // ┼
            }
        }
    }

    // bottom border
    move_cursor(x, y);
    cout << SYMBOL_DOUBLE_BOTTOM_LEFT;

    for (int i = 0; i < 9; ++i) {
        cout << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL;
        if (i == 8) cout << SYMBOL_DOUBLE_BOTTOM_RIGHT;
        else if ((i+1) % 3 == 0) cout << "\u2569"; // ╩
        else cout << "\u2567"; // ╧
    }
}

void Terminal::drawCoords() {
    // top numbers

    setTextColor(YELLOW);
    for (int i = 0; i < 9; ++i) {
        move_cursor(boardLeft + 2 + i*4, boardTop - 1);
        cout << (char)('1' + i);
    }
    // left numbers
    for (int i = 0; i < 9; ++i) {
        move_cursor(boardLeft - 2, boardTop + 1 + i*2);
        cout << (char)('1' + i);
    }

    resetTextColor();
}

void Terminal::drawInstructions() {
    int col = boardLeft + BoardCols + 4;
    int row = boardTop;
    move_cursor(col, row); cout << "Controls:";
    move_cursor(col, row+2); cout << upKey << " = Up";
    move_cursor(col, row+3); cout << leftKey << " = Left";
    move_cursor(col, row+4); cout << rightKey << " = Right";
    move_cursor(col, row+5); cout << downKey << " = Down";


    move_cursor(col, row+7); cout << "i = Insert";
    move_cursor(col, row+8); cout << "p = Pencil";
    move_cursor(col, row+9); cout << "g = Go (col,row)";
    move_cursor(col, row+10); cout << "c = Check colors";
    move_cursor(col, row+11); cout << "x = Delete cell";
    move_cursor(col, row+12); cout << "SPACE = Highlight";
    move_cursor(col, row+13); cout << "TAB = switch mode";
    move_cursor(col, row+14); cout << "b = Pause/Resume";
    move_cursor(col, row+15); cout << "r = Restart";
    move_cursor(col, row+16); cout << "q = Quit";
}

void Terminal::drawPencil() {
    auto marks = game->getPencilMarks();
    auto grid = game->getPlayGrid();
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (grid[r][c] >= 1) continue; // only empty cells
            int cellY = boardTop + 1 + r*2;
            int cellX = boardLeft + 1 + c*4;
            // render three tiny pencil marks (idx mapping like reference)
            int idx[3] = {0,2,1};
            move_cursor(cellX, cellY);
            for (int k = 0; k < 3; ++k) {
                char ch = marks[r][c][ idx[k] ];
                if (!checkColors && ch - '0' == highlightNum) setTextColor(YELLOW);
                else setTextColor(GRAY);
                cout << (ch ? ch : ' ');
                resetTextColor();
            }
        }
    }
}

void Terminal::drawNumbers() {
    auto grid = game->getPlayGrid();
    auto start = game->getStartGrid();
    auto solution = game->getSolution();

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            int v = grid[r][c];
            if (v < 1) continue;
            int cellY = boardTop + 1 + r*2;
            int cellX = boardLeft + 2 + c*4;
            // erase pencil area
            move_cursor(cellX-1, cellY);
            // Clear full 3-char cell to avoid leftover cursor bars
            cout << "   ";


            move_cursor(cellX, cellY);
            // Color scheme similar to tuidoku: givens white, entries cyan/blue
            if (v == start[r][c]) setTextColor(WHITE);
            else setTextColor(CYAN);
            // optional highlight
            if (!checkColors && v == highlightNum) setTextColor(BRIGHT_YELLOW);
            // correctness check colors when toggled
            if (checkColors && v == solution[r][c]) setTextColor(BRIGHT_GREEN);
            else if (checkColors && v != solution[r][c]) setTextColor(BRIGHT_RED);
            cout << (char)('0' + v);
            resetTextColor();
        }
    }
}

void Terminal::drawMode() {
    move_cursor(boardLeft, boardTop + BoardRows + 1);
    cout << mode << endl;
}

void Terminal::drawCursor() {
    int y = boardTop + 1 + cursorRow*2;
    int x = boardLeft + 1 + cursorCol*4; // Start from left edge of cell

    // Show a centered vertical bar only for empty cells
    auto grid = game->getPlayGrid();
    if (grid[cursorRow][cursorCol] == 0) {
        move_cursor(x + 1, y);
        setTextColor(BRIGHT_WHITE);
        cout << "|";
        resetTextColor();
    }
}

void Terminal::moveCursor(int row, int col) {
    cursorRow = row;
    cursorCol = col;
}

void Terminal::changeMode(const string& s) { mode = s; }

void Terminal::check() { checkColors = !checkColors; }

void Terminal::select(int val) {
    if (val == ' ') {
        int initVal = game->getStartGrid()[cursorRow][cursorCol];
        highlightNum = (initVal == 0) ? 0 : initVal;
    } else {
        highlightNum = val - '0'; // Fix: convert char to int
    }
}

void Terminal::setElapsedSeconds(int seconds) {
    elapsedSeconds = seconds < 0 ? 0 : seconds;
}

void Terminal::resetUI() {
    checkColors = false;
    highlightNum = 0;
}
