add_executable(cpp-gb-w
        main.cpp
        Renderer.cpp Renderer.h)

target_link_libraries(cpp-gb-w PRIVATE cpp-gb debugger)
target_link_libraries(cpp-gb-w PRIVATE project_options)
target_link_libraries(cpp-gb-w PRIVATE imgui)
target_link_libraries(cpp-gb-w PRIVATE ${GLFW})
# target_link_libraries(cpp-gb-w PRIVATE ${OPENGL_gl_LIBRARY})   
target_link_libraries(cpp-gb-w PRIVATE CONAN_PKG::opengl)   
target_link_libraries(cpp-gb-w PRIVATE glad)        
target_link_libraries(cpp-gb-w PRIVATE CONAN_PKG::spdlog)
target_link_libraries(cpp-gb-w PRIVATE CONAN_PKG::fmt)
