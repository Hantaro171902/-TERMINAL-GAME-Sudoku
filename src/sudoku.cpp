#include "sudoku.hpp"
#include <array>
#include <algorithm>
#include <random>
#include <chrono>
#include <string>
#include <cstring>
#include <stdexcept>
#include <iostream>

using namespace std;

#define SIZE 9

Sudoku::Sudoku():Sudoku(81){};

Sudoku::Sudoku(const char *gridString) {
    if (strlen(gridString) != 81)
        throw invalid_argument("Wrong amount of digits for a sudoku puzzle");
    
    auto idx = 0;
    for (auto i = 0; i < 9; i++) {
        for (auto j = 0; j < 9; j++) {
            grid[i][j] = gridString[idx++] - '0';
        }
    }
    solver = Solver(gridString);
    solver.solve();
}

Sudoku::Sudoku(int maxUnknowns) {

    struct Cell
    {
        int row;
        int col;
        int val;
    };
    

    solver = Solver();
    solver.solve();
    grid = solver.getGrid();

    array<Cell, SIZE*SIZE> cells;
    int count = 0;
    for (auto i = 0; i < SIZE; i++) {
        for (auto j = 0; j < SIZE; j++) {
            cells[count].row = i;
            cells[count].col = j;
            cells[count].val = grid[i][j];
            count++;
        }
    }

    // Shuffle array to randomly remove positions
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle (cells.begin(), cells.end(), default_random_engine(seed));

    int unknowns = 0;
    for (auto cell: cells) {
        grid[cell.row][cell.col] = 0;
        solver = Solver(grid);
        solver.solve();
        if (!solver.isUnique()) {
            // Removal of value does not produce unique solution
            // Put it back
            grid[cell.row][cell.col] = cell.val;
        }
        else
            unknowns++;
        if (unknowns == maxUnknowns)
            break;
    }

    // Solve again incase last removal broke the puzzle
    solver = Solver(grid);
    solver.solve();
}


array<array<int, 9>, 9> Sudoku::getGrid() {
    return grid;
}

array<array<int, 9>, 9> Sudoku::getSolution() {
    return solver.getGrid();
}
