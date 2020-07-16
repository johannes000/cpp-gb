cmake_minimum_required(VERSION 3.16)
project(cpp-gb)

set(CMAKE_CXX_STANDARD 17)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)


# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)


if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -Wcast-align -Wnull-dereference -Woverloaded-virtual  -Wformat=2")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


find_library(SDL SDL2 PATHS "./external/SDL2-2.0.12/x86_64-w64-mingw32/lib")
find_library(SDLmain SDL2main PATHS "./external/SDL2-2.0.12/x86_64-w64-mingw32/lib")
include_directories("./external/SDL2-2.0.12/x86_64-w64-mingw32/include/SDL2")

find_library(GLFW glfw3 PATHS "./external/glfw-3.3.2.bin.WIN32/lib-mingw-w64")
include_directories("./external/glfw-3.3.2.bin.WIN32/include")

add_library("glad" "./external/glad/src/glad.c")
include_directories("./external/glad/include")

include_directories("./external/imgui")
include_directories(SYSTEM "./external/imgui-filebrowser")

find_package(OpenGL REQUIRED)


include_directories(scr)

add_library(imgui
        ./external/imgui/imconfig.h
        ./external/imgui/imgui_demo.cpp
        ./external/imgui/imgui_draw.cpp
        ./external/imgui/imgui_impl_glfw.cpp
        ./external/imgui/imgui_impl_glfw.h
        ./external/imgui/imgui_impl_opengl3.cpp
        ./external/imgui/imgui_impl_opengl3.h
        ./external/imgui/imgui_internal.h
        ./external/imgui/imgui_widgets.cpp
        ./external/imgui/imgui.cpp
        ./external/imgui/imgui.h
        ./external/imgui/imstb_rectpack.h
        ./external/imgui/imstb_textedit.h
        ./external/imgui/imstb_truetype.h
        )

add_executable(cpp-gb
        win/main.cpp
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
        scr/Debugger/Debugger_Init.cpp
        scr/Debugger/Debug_helper.cpp

        win/Renderer.cpp win/Renderer.h
        external/imgui-filebrowser/imfilebrowser.h)


target_link_libraries(cpp-gb
        -lmingw32
        -lSDL2main
        -lSDL2
        imgui
        ${SDL}
        ${SDLmain}
        ${OPENGL_gl_LIBRARY}
        ${GLFW}
        "glad"
        )
