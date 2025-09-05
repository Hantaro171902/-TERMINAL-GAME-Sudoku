#include "game.hpp"
#include "stop_watch.hpp"
#include "utils.hpp"
#include <iostream>

using namespace std;

Game::Game(Board b, const char* navKeys) : board(b), terminal(&board, navKeys) {
    mode = 'i';
    row = 0;
    col = 0;
    paused = false;
    elapsedSeconds = 0;
    startTime = std::chrono::steady_clock::now();
    pausedAccumulated = std::chrono::seconds(0);
    lastDisplayedSeconds = -1;
}

static bool isDigitOrSpace(int ch) {
    return ((ch > '0' && ch <= '9') || ch == ' ');
}

int Game::readKey() {
    int ch = getch();
    if (ch == 27) { // ESC sequence for arrows: ESC [ A/B/C/D
        if (kbhit()) {
            int ch2 = getch();
            if (ch2 == '[') {
                int ch3 = getch();
                switch (ch3) {
                    case 'A': return 'W'; // up
                    case 'B': return 'S'; // down
                    case 'C': return 'D'; // right
                    case 'D': return 'A'; // left
                }
            }
        }
        return 27;  // bare ESC (no longer used for mode toggle)
    }
    return ch;
}

int Game::pollKey() {
#ifdef _WIN32
    if (kbhit()) return readKey();
    return -1;
#else
    if (kbhit()) return readKey();
    return -1;
#endif
}

void Game::mainLoop() {
    board.startPlaying();
    terminal.printBoard();

    while (board.isPlaying()) {
        // compute elapsed seconds precisely from steady_clock
        if (!paused) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                now - startTime - pausedAccumulated
            );
            elapsedSeconds = (int)elapsed.count();
        }
        terminal.setElapsedSeconds(elapsedSeconds);

        // only redraw when the visible seconds change (or on input handled later)
        if (elapsedSeconds != lastDisplayedSeconds) {
            terminal.printBoard();
            lastDisplayedSeconds = elapsedSeconds;
        }

        int ch = pollKey();
        if (ch == -1) {
            // small sleep to avoid busy loop
            sleep_ms(30);
            continue;
        }
        // ignore any non-control char inputs completely unless digit/space
        switch (ch) {
            case 'a': left(); break;
            case 's': down(); break;
            case 'w': up(); break;
            case 'd': right(); break;
            case 'g': changeMode('g'); terminal.printBoard(); go(); changeMode('i'); break;
            case 'i': changeMode('i'); break;
            case 'p': changeMode('p'); break;
            case '\t': // TAB toggles insert/pencil
                changeMode(mode == 'i' ? 'p' : 'i');
                terminal.printBoard();
                break;
            case 'b': // pause/resume
                togglePause();
                terminal.printBoard();
                break;
            case 'r':
            case 'R':
                restart();
                terminal.printBoard();
                break;
            case 'x':
            case 'X':
                changeMode('x');
                if (!paused) insert(' '); // clear current cell
                terminal.printBoard();
                changeMode('i');
                break;
            case 'q': board.stopPlaying(); break;
            case 'c': terminal.check(); terminal.printBoard(); break;
            default:
                if (isDigitOrSpace(ch)) {
                    if (!paused) {
                        terminal.select(ch);
                        if (mode == 'i') insert((char)ch);
                        else if (mode == 'p') pencil((char)ch);
                    }
                    terminal.printBoard();
                    lastDisplayedSeconds = elapsedSeconds;
                }
                // otherwise, ignore silently (no redraw needed)
        }
        if (mode == 'i' || mode == 'p') terminal.moveCursor(row, col);
        if (board.isWon()) break;
    }
    if (!board.isWon()) return;

    // show final elapsed time
    terminal.changeMode("Time taken: " + formatTime(elapsedSeconds));
    terminal.printBoard();
    getch();
}

void Game::insert(char val) { board.insert(val, row, col); }
void Game::pencil(char val) { board.pencil(val, row, col); }

void Game::changeMode(char c) {
    mode = c;
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
    while (c < '1' || c > '9') { 
        c = (char)getch(); 
        if (c=='q') 
        return; 
    }
    while (r < '1' || r > '9') { 
        r = (char)getch(); 
        if (r=='q') 
        return; 
    }
    row = r - '1'; 
    col = c - '1';
    terminal.moveCursor(row, col);
}

void Game::togglePause() {
    if (!paused) {
        paused = true;
        pausedSince = std::chrono::steady_clock::now();
        terminal.changeMode(std::string("Paused"));
    } else {
        paused = false;
        auto now = std::chrono::steady_clock::now();
        pausedAccumulated += std::chrono::duration_cast<std::chrono::seconds>(now - pausedSince);
        terminal.changeMode(std::string("Insert mode"));
    }
}

void Game::restart() {
    board.reset();
    row = 0; col = 0;
    mode = 'i';
    paused = false;
    elapsedSeconds = 0;
    startTime = std::chrono::steady_clock::now();
    pausedAccumulated = std::chrono::seconds(0);
    lastDisplayedSeconds = -1;
    terminal.resetUI();
    terminal.moveCursor(row, col);
}

