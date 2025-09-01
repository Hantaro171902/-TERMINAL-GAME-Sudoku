#pragma once
#include "solver.hpp"

class Sudoku {
private:
    std::array<std::array<int, 9>, 9> grid;
    Solver solver;
    
public:
    Sudoku();
    Sudoku(int maxUnknowns);
    Sudoku(const char *gridString);
    std::array<std::array<int, 9>, 9> getGrid();
    std::array<std::array<int, 9>, 9> getSolution();
};