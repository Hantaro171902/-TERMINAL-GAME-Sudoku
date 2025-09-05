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
    Terminal terminal;
    char mode; // 'i' insert, 'p' pencil, 'g' go
    int row, col; // cursor pos
    // timer state
    bool paused = false;
    int elapsedSeconds = 0; // last computed value for display
    std::chrono::steady_clock::time_point startTime; // when game started or restarted
    std::chrono::steady_clock::time_point pausedSince; // when pause began
    std::chrono::seconds pausedAccumulated{0}; // total paused duration
    int lastDisplayedSeconds = -1; // to throttle redraws
    void togglePause();
    void restart();
    int pollKey(); // returns -1 if no key available
};