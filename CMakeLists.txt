cmake_minimum_required(VERSION 3.16)
project(cpp-gb LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)


# allow for static analysis options
# include(cmake/StaticAnalyzers.cmake)

include(cmake/Doxygen.cmake)
enable_doxygen()


set(CONAN_EXTRA_REQUIRES  "glfw/3.3.4
                           opengl/system
                           imgui/1.82
                           ")

include(cmake/Conan.cmake)
run_conan()

# if (NOT MSVC)
#     set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wnon-virtual-dtor -Wcast-align -Wnull-dereference -Woverloaded-virtual  -Wformat=2 ")
# else()
if(MSVC)
    set(CMAKE_CXX_FLAGS "/W3 /EHa ")
endif ()

include_directories(${CONAN_INCLUDE_DIRS})
include_directories(${CONAN_SRC_DIRS_IMGUI})
include_directories(${CONAN_LIB_DIRS_IMGUI})
include_directories(${CONAN_BUILD_DIRS_SPDLOG})
include_directories(${CONAN_INCLUDE_DIRS_SPDLOG})
include_directories("./scr/Debugger")
include_directories("./scr/CPU")
include_directories("./scr")
include_directories("./win")
include_directories("./test")
include_directories("./external/miniaudio-master")

include_directories("./external/glad/include/")
add_library("glad" STATIC "./external/glad/src/glad.c")

find_library(DSound dsound.lib)

option(PACKAGE_TESTS "Build the tests" ON)

if(PACKAGE_TESTS)
        enable_testing()
        include(GoogleTest)
        add_subdirectory(external/googletest)
        add_subdirectory(test)
endif()
# find_package(OpenGL REQUIRED)

add_library(imgui_bindings
       ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_opengl3.cpp
       ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_opengl3.h
       ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_glfw.cpp
       ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_glfw.h
        )


add_subdirectory(win)

