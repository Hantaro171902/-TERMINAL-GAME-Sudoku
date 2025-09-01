cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TerminalSudoku VERSION 1.0)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set source and header files
set(SOURCES
    src/main.cpp
    src/utils.cpp
    src/board.cpp
    src/game.cpp
    src/sudoku.cpp
    src/solver.cpp
    src/terminal.cpp
    src/stop_watch.cpp
    src/color.cpp
)

set(HEADERS
    src/headers/utils.hpp
    src/headers/board.hpp
    src/headers/game.hpp
    src/headers/sudoku.hpp
    src/headers/solver.hpp
    src/headers/terminal.hpp
    src/headers/stop_watch.hpp
    src/headers/color.hpp
)

# Exe
add_executable(Sudoku ${SOURCES} ${HEADERS})

target_include_directories(Sudoku PRIVATE src/headers)

# Optional: copy asset folder into build dir
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
