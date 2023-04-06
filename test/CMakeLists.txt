set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_executable(gb_test test.cpp)

target_link_libraries(gb_test PRIVATE imgui_bindings)
target_link_libraries(gb_test PRIVATE gtest)

add_test(NAME gb_test COMMAND gb_test)
# add_custom_command( TARGET  gb_test
#                 POST_BUILD
#                 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.exe
#                 )