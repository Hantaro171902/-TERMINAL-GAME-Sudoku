#include "game.hpp"
#include "stop_watch.hpp"
#include "utils.hpp"
#include <iostream>

using namespace std;

Game::Game(Board b, const char* navKeys) : board(b), terminal(&board, navKeys) {
    node = 'i';
    row = 0;
    col = 0;
}

static bool isDigitOrSpace(int ch) {
    return ((ch > '0' && ch < '9') || ch == ' ');
}

int Game::readKey() {
    int ch = getch();
    if (ch == 27) { // ESC sequence for arrows: ESC [ A/B/C/D
        if (kbhit()) {
            int ch2 = getch();
            if (ch2 == '[') {
                int ch3 = getch();
                switch (ch3) {
                    case 'A': return 'K'; // up
                    case 'B': return 'J'; // down
                    case 'C': return 'L'; // right
                    case 'D': return 'H'; // left
                }
            }
        }
        return 27;  // bare ESC
    }
    return ch;
}

void Game::mainLoop() {
    board.startPlaying();
    Stopwatch timer;
    timer.start();

    terminal.printBoard();

    while (board.isPlaying()) {
        int ch = readKey();
        switch (ch) {
            case 'a': case 'h': case 'H': left(); break;
            case 's': case 'j': case 'J': down(); break;
            case 'w': case 'k': case 'K': up(); break;
            case 'd': case 'l': case 'L': right(); break;
            case 'g': changeMode('g'); terminal.printBoard(); go(); changeMode('i'); break;
            case 'i': changeMode('i'); break;
            case 'p': changeMode('p'); break;
            case 'q': board.stopPlaying(); break;
            case 'c': terminal.check(); terminal.printBoard(); break;
            case 27: // ESC toggle
                changeMode(mode == 'i' ? 'p' : 'i');
                terminal.printBoard();
                break;
            default:
                if (isDigitOrSpace(ch)) {
                    terminal.select(ch);
                    if (mode == 'i') insert((char)ch);
                    else if (mode == 'p') pencil((char)ch);
                    terminal.printBoard();
                }
        }
        if (mode == 'i' || mode == 'p') terminal.moveCursor(row, col);
        if (board.isWon()) break;
    }

    timer.stop();
    if (!board.isWon()) return;

    terminal.changeMode(timer.timeTaken());
    terminal.printBoard();
    getch();
}

void Game::insert(char val) { board.insert(val, row, col); }
void Game::pencil(char val) { board.pencil(val, row, col); }

void Game::changeMode(char c) {
    node = c;
    switch (c) {
        case 'i': terminal.changeMode("Insert mode"); break;
        case 'p': terminal.changeMode("Pencil mode"); break;
        case 'g': terminal.changeMode("Go"); break;
    }
}

void Game::up() { 
    if (row <= 0) row = 9; 
    terminal.moveCursor(--row, col); 
    terminal.printBoard(); 
}

void Game::down() { 
    if (row >= 8) row = -1; 
    terminal.moveCursor(++row, col); 
    terminal.printBoard(); 
}

void Game::left() { 
    if (col <= 0) col = 9; 
    terminal.moveCursor(row, --col); 
    terminal.printBoard(); 
}

void Game::right() { 
    if (col >= 8) col = -1; 
    terminal.moveCursor(row, ++col); 
    terminal.printBoard(); 
}

void Game::go() {
    char c = 0, r = 0;
    while (c < '1' || c > '9') { c = (char)getch(); if (c=='q') return; }
    while (r < '1' || r > '9') { r = (char)getch(); if (r=='q') return; }
    row = r - '1'; col = c - '1';
    terminal.moveCursor(row, col);
}

