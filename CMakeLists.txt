cmake_minimum_required(VERSION 3.16)
project(cpp-gb)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_DIR "C:\\Users\\knaub\\source\\SDL2-2.0.12\\i686-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

include_directories(scr)

add_executable(cpp-gb
        scr/main.cpp
        scr/CPU/ARITHMETIC.cpp
        scr/CPU/CONTROL.cpp
        scr/CPU/helper.cpp
        scr/CPU/JUMPS.cpp
        scr/CPU/LD.cpp
        scr/CPU/SHIFT.cpp
        scr/CARDRIDGE.cpp scr/CARDRIDGE.h
        scr/CPU.cpp scr/CPU.h
        scr/gameboy.cpp scr/gameboy.h
        scr/MEMORY.cpp scr/MEMORY.h
        scr/PPU.cpp scr/PPU.h
        scr/type.cpp scr/type.h)

target_link_libraries(cpp-gb -lmingw32 -lSDL2main -lSDL2 ${SDL2_LIBRARY})
