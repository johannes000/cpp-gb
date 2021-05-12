cmake_minimum_required(VERSION 3.16)
project(cpp-gb LANGUAGES CXX C)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/external/modules/")
# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

# enable cache system
include(cmake/Cache.cmake)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

include(cmake/Doxygen.cmake)
enable_doxygen()

include(cmake/Conan.cmake)
run_conan()

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wnon-virtual-dtor -Wcast-align -Wnull-dereference -Woverloaded-virtual  -Wformat=2 ")
else()
    set(CMAKE_CXX_FLAGS "/W3 ")
endif ()
set(LLVM_ENABLE_RTTI ON)
set(LLVM_ENABLE_EH ON)

if (MSVC)
        find_library(GLFW glfw3 PATHS "./external/glfwWIN64/lib-vc2019")
else (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        # find_library(GLFW glfw3 PATHS "./external/glfwWIN64/lib-vc2019")
        find_library(GLFW glfw3 PATHS "./external/glfwWIN64/lib-mingw-w64")
endif ()

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)

include_directories("./external/googletest/googletest/include")
include_directories("./external/spdlog/include")
include_directories("./external/fmt/include")
include_directories("./external/glfwWIN64/include")
include_directories("./external/imgui")
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

add_subdirectory(win)

