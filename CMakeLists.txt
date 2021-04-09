cmake_minimum_required(VERSION 3.16)
project(cpp-gb LANGUAGES CXX C)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/external/modules/")


# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# # enable cache system
# include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

# include(cmake/Conan.cmake)
# run_conan()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -Wcast-align -Wnull-dereference -Woverloaded-virtual  -Wformat=2")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif ()

add_subdirectory("./external")
include_directories("./external/imgui")
include_directories("./external/imgui-filebrowser")
include_directories("./external/glad/include/")
include_directories("./external/glfw-3.3.2.bin.WIN32/include")
include_directories("./scr")
include_directories("./scr/Debugger")
include_directories("./win")

add_library(cpp-gb
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
)

add_library(debugger
        scr/Debugger/Debugger.cpp scr/Debugger/Debugger.h
        scr/Debugger/Debug_helper.cpp
)

add_executable(cpp-gb-64
        win/main.cpp
        win/Renderer.cpp win/Renderer.h
        external/imgui-filebrowser/imfilebrowser.h)


target_link_libraries(cpp-gb-64 cpp-gb debugger)
target_link_libraries(cpp-gb-64 imgui)
target_link_libraries(cpp-gb-64 ${GLFW})
target_link_libraries(cpp-gb-64 ${OPENGL_gl_LIBRARY} "glad")        