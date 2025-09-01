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
    src/generator.cpp
    src/solver.cpp
    src/terminal.cpp
    src/stop_watch.cpp
    src/color.cpp
)

set(HEADERS
    src/headers/utils.hpp
    src/headers/disk.hpp
    src/headers/rod.hpp
    src/headers/game_service.hpp
    src/headers/log_service.hpp
    src/headers/design_char_constants.hpp
    src/headers/tower_of_hanoi.hpp
    src/headers/color.hpp
    src/headers/cursor_input.hpp
)

# Exe
add_executable(TowerOfHanoi ${SOURCES} ${HEADERS})

target_include_directories(TowerOfHanoi PRIVATE src/headers)

# Optional: copy asset folder into build dir
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
