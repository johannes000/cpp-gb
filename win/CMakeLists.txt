link_directories(${PROJECT_SOURCE_DIR}/lib/Win64/)

set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/include/imgui/backends)
add_library(imgui_bindings
       ${IMGUI_DIR}/imgui_impl_opengl3.cpp
       ${IMGUI_DIR}/imgui_impl_opengl3.h
       ${IMGUI_DIR}/imgui_impl_glfw.cpp
       ${IMGUI_DIR}/imgui_impl_glfw.h
        )

file(GLOB PLATFORM_SOURCES 
    ${CMAKE_CURRENT_LIST_DIR}/Audio.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/Renderer.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp 
    )

message(${CMAKE_CURRENT_LIST_DIR})

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY SOURCES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

add_executable(cpp-gb-w ${PLATFORM_SOURCES} ${SOURCES})

target_link_libraries(
        cpp-gb-w PRIVATE 
                DSound
                imgui_bindings
                glfw3
                OpenGL32
                )

file(COPY "${PROJECT_SOURCE_DIR}/lib/Win64/" DESTINATION "${EXECUTABLE_OUTPUT_PATH}" FILES_MATCHING PATTERN "*.dll")
file(COPY "${PROJECT_SOURCE_DIR}/roms/" DESTINATION "${EXECUTABLE_OUTPUT_PATH}/roms")