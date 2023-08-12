cmake_minimum_required(VERSION 3.16)
project(cpp-gb VERSION 0.1)

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

set(BUILD_PATH ${PROJECT_SOURCE_DIR}/build/bin/Debug)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_PATH})
set(LIBRARY_OUTPUT_PATH ${BUILD_PATH})

# find_library(DSound dsound.lib)
# find_library(SDL2 SDL2.lib "./external/SDL2-2.0.22/VC/lib/x64")
include_directories(${PROJECT_SOURCE_DIR}/scr/)
include_directories(${PROJECT_SOURCE_DIR}/win/)
include_directories(${PROJECT_SOURCE_DIR}/test/)
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/include/imgui/)

# add dependencies
include(cmake/CPM.cmake)

CPMAddPackage(
  NAME SDL
  GITHUB_REPOSITORY libsdl-org/SDL
  GIT_TAG release-2.26.5
)
CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 10.0.0
)

CPMAddPackage(
  NAME imgui
  GITHUB_REPOSITORY ocornut/imgui
  GIT_TAG v1.89.8-docking
)
CPMAddPackage(
  NAME glew
  GITHUB_REPOSITORY nigels-com/glew
  GIT_TAG glew-2.2.0
)
CPMAddPackage(
  NAME glm
  GITHUB_REPOSITORY g-truc/glm
  GIT_TAG 0.9.9.8
)

include(FetchContent)
FetchContent_Declare(LoguruGitRepo
  GIT_REPOSITORY "https://github.com/emilk/loguru" # can be a filesystem path
  GIT_TAG "master"
)

# set any loguru compile-time flags before calling MakeAvailable()
set(LOGURU_USE_FMTLIB TRUE)
FetchContent_MakeAvailable(LoguruGitRepo) # defines target 'loguru::loguru'

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp # optionally comment this out
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

include_directories(imgui INTERFACE
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
  ${SDL_SOURCE_DIR}/include
)

target_compile_definitions(imgui PUBLIC -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS) # optional imgui setting
set_target_properties(imgui PROPERTIES FOLDER third-party) # optoinal IDE dir

file(GLOB SOURCES
  scr/APU.cpp
  scr/CARTRIDGE.cpp
  scr/CONTROLS.cpp
  scr/CPU.cpp
  scr/CPU/ARITHMETIC.cpp
  scr/CPU/CONTROL.cpp
  scr/CPU/helper.cpp
  scr/CPU/JUMPS.cpp
  scr/CPU/LD.cpp
  scr/CPU/SHIFT.cpp
  scr/MEMORY.cpp
  scr/PPU.cpp

  scr/gameboy.cpp

  scr/main.cpp
)

# find_package(OpenGL REQUIRED)

# add_executable(cpp-gb-w main.cpp ${IMG})
add_executable(cpp-gb-w ${SOURCES})

target_link_libraries(
  cpp-gb-w PRIVATE
  fmt
  loguru::loguru
  glm::glm
  imgui
  opengl32
  SDL2::SDL2
)

file(COPY "${PROJECT_SOURCE_DIR}/roms/" DESTINATION "${EXECUTABLE_OUTPUT_PATH}/roms")