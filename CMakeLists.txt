cmake_minimum_required(VERSION 3.16)
project(cpp-gb LANGUAGES CXX C)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/external/modules/")


# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

add_subdirectory(./external/docopt)
if(NOT TARGET docopt)
# Stand-alone build
        set(docopt_DIR "./external/docopt")
        list(APPEND CMAKE_MODULE_PATH "${docopt_DIR}/cmake")
        find_package(docopt COMPONENTS CXX REQUIRED)
endif()

add_subdirectory(./external/spdlog)
if(NOT TARGET spdlog)
# Stand-alone build
        set(SPDLOG_DIR "./external/spdlog")
        list(APPEND CMAKE_MODULE_PATH "${SPDLOG_DIR}/cmake")
        find_package(SPDLOG REQUIRED)
endif()

add_subdirectory(./external/fmt)
if(NOT TARGET fmt)
# Stand-alone build
        set(FMT_DIR "./external/fmt")
        list(APPEND CMAKE_MODULE_PATH "${FMT_DIR}/build")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wnon-virtual-dtor -Wcast-align -Wnull-dereference -Woverloaded-virtual  -Wformat=2 ")
else()
    set(CMAKE_CXX_FLAGS "/W3 /external:I")

endif ()

if (MSVC)
        find_library(GLFW glfw3 PATHS "./external/glfwWIN64/lib-vc2019")
else ()
        find_library(GLFW glfw3 PATHS "./external/glfwWIN64/lib-mingw-w64")
endif ()

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)

include_directories("./external/googletest/googletest/include")
include_directories("./external/spdlog/include")
include_directories("./external/fmt/include")
include_directories("./external/docopt")
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

find_package(OpenGL REQUIRED)
# find_package(imgui REQUIRED)

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

