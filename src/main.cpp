#include <cstring>
#include <string>
#include <fstream>
#include <chrono>
#include <random>
#include <iostream>
#include "Generator.h"
#include "Board.h"
#include "GameTerm.hpp"


static void printHelpTerm() {
std::cout <<
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


static std::string getRandomXMLPuzzle(const char *fileName) {
std::ifstream file(fileName);
std::string line; std::vector<std::string> puzzles;
while (std::getline(file, line)) {
size_t found = line.find("<game data=");
if (found == std::string::npos) continue;
size_t begin = line.find('\''); if (begin == std::string::npos) begin = line.find('"');
size_t end = line.rfind('\''); if (end == std::string::npos) end = line.rfind('"');
if (begin != std::string::npos && end != std::string::npos && end>begin) {
puzzles.emplace_back(line.substr(begin+1, end-begin-1));
}
}
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
srand((unsigned)seed);
return puzzles.empty() ? std::string(81,'0') : puzzles[rand() % puzzles.size()];
}


static std::string getRandomSdmPuzzle(const char *fileName) {
std::ifstream file(fileName);
std::vector<std::string> puzzles; std::string line;
while (std::getline(file, line)) puzzles.push_back(line);
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
srand((unsigned)seed);
return puzzles.empty() ? std::string(81,'0') : puzzles[rand() % puzzles.size()];
}


int main(int argc, char* argv[]) {
char navKeys[5] = "hjkl"; // default
Generator* gen = nullptr;


for (int i = 1; i < argc; ++i) {
if (std::atoi(argv[i]) > 0) gen = new Generator(std::atoi(argv[i]));
else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--wasd") == 0) { strcpy(navKeys, "aswd"); }
else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) { printHelpTerm(); return 0; }
else if (strcmp(argv[i], "--opensudoku") == 0 && !gen && i+1 < argc) { gen = new Generator(getRandomXMLPuzzle(argv[i+1]).c_str()); ++i; }
else if (strcmp(argv[i], "--sdm") == 0 && !gen && i+1 < argc) { gen = new Generator(getRandomSdmPuzzle(argv[i+1]).c_str()); ++i; }
}


if (!gen) gen = new Generator();
Board board(*gen);
GameTerm game(board, navKeys);
game.mainLoop();
delete gen;
return 0;
}