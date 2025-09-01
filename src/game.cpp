#include "GameTerm.hpp"
int ch = readKey();
switch (ch) {
case 'a': case 'h': case 'H': left(); break;
case 's': case 'j': case 'J': down(); break;
case 'w': case 'k': case 'K': up(); break;
case 'd': case 'l': case 'L': right(); break;
case 'g': changeMode('g'); window.printBoard(); go(); changeMode('i'); break;
case 'i': changeMode('i'); break;
case 'p': changeMode('p'); break;
case 'q': board.stopPlaying(); break;
case 'c': window.check(); window.printBoard(); break;
case 27: // ESC toggle
changeMode(mode == 'i' ? 'p' : 'i');
window.printBoard();
break;
default:
if (isDigitOrSpace(ch)) {
window.select(ch);
if (mode == 'i') insert((char)ch);
else if (mode == 'p') pencil((char)ch);
window.printBoard();
}
}
if (mode == 'i' || mode == 'p') window.moveCursor(row, col);
if (board.isWon()) break;
}


timer.stop();
if (!board.isWon()) return;


window.changeMode(timer.timeTaken());
window.printBoard();
getch();
}


void GameTerm::insert(char val) { board.insert(val, row, col); }
void GameTerm::pencil(char val) { board.pencil(val, row, col); }


void GameTerm::changeMode(char c) {
mode = c;
switch (c) {
case 'i': window.changeMode("Insert mode"); break;
case 'p': window.changeMode("Pencil mode"); break;
case 'g': window.changeMode("Go"); break;
}
}


void GameTerm::up() { if (row <= 0) row = 9; window.moveCursor(--row, col); window.printBoard(); }
void GameTerm::down() { if (row >= 8) row = -1; window.moveCursor(++row, col); window.printBoard(); }
void GameTerm::left() { if (col <= 0) col = 9; window.moveCursor(row, --col); window.printBoard(); }
void GameTerm::right() { if (col >= 8) col = -1; window.moveCursor(row, ++col); window.printBoard(); }


void GameTerm::go() {
char c = 0, r = 0;
while (c < '1' || c > '9') { c = (char)getch(); if (c=='q') return; }
while (r < '1' || r > '9') { r = (char)getch(); if (r=='q') return; }
row = r - '1'; col = c - '1';
window.moveCursor(row, col);
}