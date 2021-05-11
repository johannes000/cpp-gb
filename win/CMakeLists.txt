add_executable(cpp-gb-w
        main.cpp
        Renderer.cpp Renderer.h)

target_link_libraries(cpp-gb-w PUBLIC cpp-gb debugger)

target_link_libraries(cpp-gb-w PUBLIC project_options)
target_link_libraries(cpp-gb-w PUBLIC imgui)
target_link_libraries(cpp-gb-w PUBLIC ${GLFW})
target_link_libraries(cpp-gb-w PUBLIC ${OPENGL_gl_LIBRARY} "glad")        
target_link_libraries(cpp-gb-w PUBLIC spdlog::spdlog)
target_link_libraries(cpp-gb-w PUBLIC fmt::fmt)
target_link_libraries(cpp-gb-w PUBLIC docopt)
