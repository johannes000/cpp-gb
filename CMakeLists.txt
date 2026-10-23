cmake_minimum_required(VERSION 3.21)
project(cpp-gb VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(CPP_GB_BUILD_TESTS "Blargg-CPU-Tests (GoogleTest) bauen" OFF)

# ---------------------------------------------------------------------------
# Alles statisch: keine DLLs / .so ausser den System-Bibliotheken
# ---------------------------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(WIN32)
  # MinGW / clang (mingw): Laufzeit direkt in die exe linken
  add_link_options(-static -static-libgcc -static-libstdc++)
else()
  add_link_options(-static-libgcc -static-libstdc++)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# ---------------------------------------------------------------------------
# Abhaengigkeiten
# ---------------------------------------------------------------------------
# Neuere clang-Versionen (z.B. MSYS2 clang64, zig) erkennen das consteval-Format-
# string-Checking des mitgelieferten fmt nicht als konstant -> Pruefung abschalten
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_definitions(FMT_CONSTEVAL=)
endif()

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME SDL3
  GITHUB_REPOSITORY libsdl-org/SDL
  GIT_TAG release-3.2.30
  OPTIONS
    "SDL_SHARED OFF"
    "SDL_STATIC ON"
    "SDL_TESTS OFF"
    "SDL_TEST_LIBRARY OFF"
    "SDL_EXAMPLES OFF"
    "SDL_INSTALL OFF"
    "SDL_FORCE_STATIC_VCRT ON"
)

CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  VERSION 1.14.1
  OPTIONS
    "SPDLOG_BUILD_SHARED OFF"
    "SPDLOG_BUILD_EXAMPLE OFF"
    "SPDLOG_BUILD_TESTS OFF"
    "SPDLOG_INSTALL OFF"
)

CPMAddPackage(
  NAME imgui
  GITHUB_REPOSITORY ocornut/imgui
  GIT_TAG v1.91.9-docking
  DOWNLOAD_ONLY YES
)

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_compile_definitions(imgui PUBLIC IMGUI_DISABLE_OBSOLETE_FUNCTIONS)
target_link_libraries(imgui PUBLIC SDL3::SDL3-static)
set_target_properties(imgui PROPERTIES FOLDER third-party)

# ---------------------------------------------------------------------------
# Emulator-Kern (ohne Fenster, laeuft auch in Tests)
# ---------------------------------------------------------------------------
add_library(gb_core STATIC
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
  scr/Debugger/Debugger.cpp
  scr/Debugger/Debug_helper.cpp
)
target_include_directories(gb_core PUBLIC ${PROJECT_SOURCE_DIR}/scr)
target_link_libraries(gb_core PUBLIC spdlog::spdlog imgui)

# ---------------------------------------------------------------------------
# Anwendung
# ---------------------------------------------------------------------------
add_executable(cpp-gb
  scr/main.cpp
  scr/Audio.cpp
  scr/GUI/Gui.cpp
)
target_link_libraries(cpp-gb PRIVATE gb_core imgui SDL3::SDL3-static)

if(WIN32)
  target_link_libraries(cpp-gb PRIVATE opengl32)
else()
  find_package(OpenGL QUIET)
  if(TARGET OpenGL::GL)
    target_link_libraries(cpp-gb PRIVATE OpenGL::GL)
  else()
    message(WARNING "OpenGL-Entwicklungspakete nicht gefunden (z.B. libgl-dev installieren)")
    target_link_libraries(cpp-gb PRIVATE GL)
  endif()
endif()

# Warnungen
if(MSVC)
  set(GB_WARNINGS /W4)
else()
  set(GB_WARNINGS -Wall -Wextra -Wno-unused-variable -Wno-unused-but-set-variable)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND GB_WARNINGS -Wno-unused-parameter -Wno-unused-private-field)
  endif()
endif()
target_compile_options(gb_core PRIVATE ${GB_WARNINGS})
target_compile_options(cpp-gb PRIVATE ${GB_WARNINGS})

# ROMs (liegen nicht im Repository) neben die exe kopieren, falls vorhanden
if(EXISTS ${PROJECT_SOURCE_DIR}/roms)
  add_custom_command(
    TARGET cpp-gb POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${PROJECT_SOURCE_DIR}/roms $<TARGET_FILE_DIR:cpp-gb>/roms
  )
endif()

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
if(CPP_GB_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
