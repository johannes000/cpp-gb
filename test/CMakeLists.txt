CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  VERSION 1.14.0
  OPTIONS
    "INSTALL_GTEST OFF"
    "gtest_force_shared_crt ON"
)

# Verzeichnis mit Blargg's test roms (cpu_instrs/individual/*.gb, ...)
set(GB_TEST_ROMS_DIR "${PROJECT_SOURCE_DIR}/roms" CACHE PATH "Ordner mit den Test-ROMs")

add_executable(gb_test test.cpp)
target_compile_definitions(gb_test PRIVATE GB_TEST_ROMS="${GB_TEST_ROMS_DIR}/")
target_link_libraries(gb_test PRIVATE gb_core GTest::gtest)

add_test(NAME gb_test COMMAND gb_test)
