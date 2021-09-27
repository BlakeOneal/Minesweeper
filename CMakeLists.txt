cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Project3)


## In most cases better set in the CMake cache
set(SFML_DIR "C:/Users/blake/Downloads/SFML-2.5.1-windows-gcc-7.3.0-mingw-32-bit/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
add_executable(Minesweeper main.cpp Board.cpp Board.h Tile.cpp Tile.h TextureManager.cpp TextureManager.h Random.cpp Random.h)
target_link_libraries(Minesweeper sfml-graphics sfml-audio)