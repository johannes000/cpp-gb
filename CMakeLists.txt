cmake_minimum_required(VERSION 3.16)
project(cpp-gb)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-variable -Wno-unused-but-set-variable")
  if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER_EQUAL 10.2)
    # Don't bother adding the coroutine flag when using GCC 10.0 or 10.1
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
  endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-private-field")
  if(${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER_EQUAL 10)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines-ts")
  endif()
endif()

include_directories(${PROJECT_SOURCE_DIR}/scr/)
include_directories(${PROJECT_SOURCE_DIR}/win/)
include_directories(${PROJECT_SOURCE_DIR}/test/)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/imgui/)

set(BUILD_PATH ${PROJECT_SOURCE_DIR}/build/bin/Debug)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_PATH})
set(LIBRARY_OUTPUT_PATH ${BUILD_PATH})

# find_library(DSound dsound.lib)
# find_library(SDL2 SDL2.lib "./external/SDL2-2.0.22/VC/lib/x64")

file(GLOB SOURCES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp 
    ${CMAKE_SOURCE_DIR}/src/*/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*/*.c)


option(PACKAGE_TESTS "Build the tests" ON)

if(PACKAGE_TESTS)
        enable_testing()
        include(GoogleTest)
        add_subdirectory(external/googletest)
        add_subdirectory(test)
endif()
# find_package(OpenGL REQUIRED)

add_subdirectory(win)

