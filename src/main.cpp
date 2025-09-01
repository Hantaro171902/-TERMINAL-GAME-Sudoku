#include <cstring>
#include <string>
#include <fstream>
#include <chrono>
#include <random>
#include <iostream>
#include "sudoku.hpp"
#include "board.hpp"
#include "game.hpp"

using namespace std;

static void printHelpTerm() {
    cout <<
        "usage: sudoku_term [OPTIONS]\n\n"
        "Play Sudoku in the terminal (ANSI).\n\n"
        "Optional args:\n"
        "-h, --help View this page\n"
        "-w, --wasd Display WASD instead of HJKL\n"
        "--opensudoku <xml> Load random puzzle from OpenSudoku XML\n"
        "--sdm <txt> Load random puzzle from .sdm list\n"
        "INTEGER Number of squares to leave empty (difficulty)\n\n"
        "Move with hjkl/wasd or arrows.\n"
        "'p' pencil, 'i' insert, 'g' go-to, 'c' check-colors, 'q' quit.\n";
}


static string getRandomXMLPuzzle(const char *fileName) {
    ifstream file(fileName);
    string line; vector<string> puzzles;
    while (getline(file, line)) {
        size_t found = line.find("<game data=");
        if (found == string::npos) continue;
        size_t begin = line.find('\''); 

        if (begin == string::npos) begin = line.find('"');
        size_t end = line.rfind('\''); 
        
        if (end == string::npos) end = line.rfind('"');
        if (begin != string::npos && end != string::npos && end>begin) {
            puzzles.emplace_back(line.substr(begin+1, end-begin-1));
        }
    }

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    srand((unsigned)seed);
    return puzzles.empty() ? string(81,'0') : puzzles[rand() % puzzles.size()];
}


static string getRandomSdmPuzzle(const char *fileName) {
    ifstream file(fileName);
    vector<string> puzzles; string line;
    while (getline(file, line)) puzzles.push_back(line);
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    srand((unsigned)seed);
    return puzzles.empty() ? string(81,'0') : puzzles[rand() % puzzles.size()];
}


int main(int argc, char* argv[]) {
    char navKeys[5] = "hjkl"; // default
    Sudoku* gen = nullptr;

    for (int i = 1; i < argc; ++i) {
        if (atoi(argv[i]) > 0) gen = new Sudoku(atoi(argv[i]));
        else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--wasd") == 0) { 
            strcpy(navKeys, "aswd"); 
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) { 
            printHelpTerm(); return 0; 
        }
        else if (strcmp(argv[i], "--opensudoku") == 0 && !gen && i+1 < argc) { 
            gen = new Sudoku(getRandomXMLPuzzle(argv[i+1]).c_str()); ++i; 
        }
        else if (strcmp(argv[i], "--sdm") == 0 && !gen && i+1 < argc) { 
            gen = new Sudoku(getRandomSdmPuzzle(argv[i+1]).c_str()); ++i; 
        }
        }

    if (!gen) gen = new Sudoku();
    Board board(*gen);
    Game game(board, navKeys);
    game.mainLoop();
    delete gen;
    return 0;
}