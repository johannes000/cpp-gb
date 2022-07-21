
add_executable(cpp-gb-w main.cpp)

target_link_libraries(
        cpp-gb-w PRIVATE 
                project_warnings
                project_options
                glad        
                imgui_bindings
                CONAN_PKG::imgui
                CONAN_PKG::glfw
                CONAN_PKG::opengl   
                CONAN_PKG::spdlog
                CONAN_PKG::fmt
                CONAN_PKG::docopt.cpp
                )