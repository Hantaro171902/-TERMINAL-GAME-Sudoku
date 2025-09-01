#pragma once
#include <string>
#include "board.hpp"
#include "terminal.hpp"


class Game {
public:
    Game(Board b, const char* navKeys);
    void mainLoop();


    private:
    void insert(char val);
    void pencil(char val);
    void changeMode(char c);


    void up();
    void down();
    void left();
    void right();


    void go(); // jump to col,row input


    // input helpers
    int readKey(); // uses getch() and parses arrows


private:
    Board board;
    Terminal window;
    char mode; // 'i' insert, 'p' pencil, 'g' go
    int row, col; // cursor pos
};