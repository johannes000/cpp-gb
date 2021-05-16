add_executable(cpp-gb-w
        main.cpp
        Renderer.cpp Renderer.h)

target_link_libraries(
        cpp-gb-w PRIVATE 
                cpp-gb debugger
                project_options
                imgui
                glad        
                CONAN_PKG::glfw
                CONAN_PKG::opengl   
                CONAN_PKG::spdlog
                CONAN_PKG::fmt
                )
