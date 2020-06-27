cmake_minimum_required(VERSION 3.16)
project(cpp-gb)

set(CMAKE_CXX_STANDARD 20)

find_library(SDL SDL2 PATHS "./external/SDL2-2.0.12/x86_64-w64-mingw32/lib")
find_library(SDLmain SDL2main PATHS "./external/SDL2-2.0.12/x86_64-w64-mingw32/lib")
include_directories("./external/SDL2-2.0.12/x86_64-w64-mingw32/include/SDL2")

find_library(GLFW glfw3 PATHS "./external/glfw-3.3.2.bin.WIN32/lib-mingw-w64")
include_directories("./external/glfw-3.3.2.bin.WIN32/include")

add_library("glad" "./external/glad/src/glad.c")
include_directories("./external/glad/include")

include_directories("./external/imgui")

find_package(OpenGL REQUIRED)


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
        scr/type.cpp scr/type.h
        scr/CONTROLS.cpp scr/CONTROLS.h
        scr/Debugger/Debugger.cpp scr/Debugger/Debugger.h

        external/imgui/imconfig.h
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_impl_glfw.cpp
        external/imgui/imgui_impl_glfw.h
        external/imgui/imgui_impl_opengl3.cpp
        external/imgui/imgui_impl_opengl3.h
        external/imgui/imgui_internal.h
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui.cpp
        external/imgui/imgui.h
        external/imgui/imstb_rectpack.h
        external/imgui/imstb_textedit.h
        external/imgui/imstb_truetype.h scr/Debugger/Debugger_Init.cpp)

target_link_libraries(cpp-gb
        -lmingw32
        -lSDL2main
        -lSDL2
        ${SDL}
        ${SDLmain}
        ${OPENGL_gl_LIBRARY}
        ${GLFW}
        "glad"
        )
