cmake_minimum_required(VERSION 3.16)
project(cpp-gb LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

# enable cache system
include(cmake/Cache.cmake)

# allow for static analysis options
# include(cmake/StaticAnalyzers.cmake)

include(cmake/Doxygen.cmake)
enable_doxygen()


set(CONAN_EXTRA_REQUIRES  "openal/1.21.0
                           glfw/3.3.4
                           opengl/system
                           imgui/1.82
                           ")

include(cmake/Conan.cmake)
run_conan()

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wnon-virtual-dtor -Wcast-align -Wnull-dereference -Woverloaded-virtual  -Wformat=2 ")
else()
    set(CMAKE_CXX_FLAGS "/W3 /EHa ")
endif ()

include_directories(${CONAN_INCLUDE_DIRS})
include_directories(${CONAN_SRC_DIRS_IMGUI})
include_directories(${CONAN_LIB_DIRS_IMGUI})
include_directories(${CONAN_BUILD_DIRS_SPDLOG})
include_directories(${CONAN_INCLUDE_DIRS_SPDLOG})
# include_directories("./external/spdlog/include")
include_directories("./scr/Debugger")
include_directories("./scr")
include_directories("./win")
include_directories("./test")

include_directories("./external/glad/include/")
add_library("glad" STATIC "./external/glad/src/glad.c")

option(PACKAGE_TESTS "Build the tests" ON)

if(PACKAGE_TESTS)
        enable_testing()
        include(GoogleTest)
        add_subdirectory(external/googletest)
        add_subdirectory(test)
endif()
# find_package(OpenGL REQUIRED)

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
        scr/CONTROLS.cpp scr/CONTROLS.h
)

add_library(debugger
        scr/Debugger/Debugger.cpp scr/Debugger/Debugger.h
        scr/Debugger/Debug_helper.cpp
)

add_library(imgui_bindings
       ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_opengl3.cpp
       ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_opengl3.h
       ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_glfw.cpp
       ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_glfw.h
        )


add_subdirectory(win)

