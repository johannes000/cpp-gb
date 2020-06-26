cmake_minimum_required(VERSION 3.16)
project(cpp-gb)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_DIR "C:\\Users\\knaub\\source\\SDL2-2.0.12\\i686-w64-mingw32")

find_library(GLFW glfw3 PATHS "./scr/Debugger/external/glfw-3.3.2.bin.WIN32/lib-mingw")
include_directories("./scr/Debugger/external/glfw-3.3.2.bin.WIN32/include/")

add_library("glad" "./scr/Debugger/external/glad/src/glad.c")
include_directories("./scr/Debugger/external/glad/include")

include_directories("./scr/Debugger/external/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

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


        scr/Debugger/external/imgui/imconfig.h
        scr/Debugger/external/imgui/imgui_demo.cpp
        scr/Debugger/external/imgui/imgui_draw.cpp
        scr/Debugger/external/imgui/imgui_impl_glfw.cpp
        scr/Debugger/external/imgui/imgui_impl_glfw.h
        scr/Debugger/external/imgui/imgui_impl_opengl3.cpp
        scr/Debugger/external/imgui/imgui_impl_opengl3.h
        scr/Debugger/external/imgui/imgui_internal.h
        scr/Debugger/external/imgui/imgui_widgets.cpp
        scr/Debugger/external/imgui/imgui.cpp
        scr/Debugger/external/imgui/imgui.h
        scr/Debugger/external/imgui/imstb_rectpack.h
        scr/Debugger/external/imgui/imstb_textedit.h
        scr/Debugger/external/imgui/imstb_truetype.h scr/Debugger/Debugger.cpp scr/Debugger/Debugger.h)

target_link_libraries(cpp-gb
        -lmingw32
        -lSDL2main
        -lSDL2
        ${SDL2_LIBRARY}
        ${GLFW}
        ${OPENGL_gl_LIBRARY}
        "glad"
        )
