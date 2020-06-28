//
// Created by knaub on 26.06.2020.
//

#ifndef CPP_GB_DEBUGGER_H
#define CPP_GB_DEBUGGER_H

#include "type.h"
#include "CPU.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cstdio>
#include <cstdlib>

#include <map>

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)

#include <glad/glad.h>          // Initialize with gladLoadGL()

#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


class CPU;

class PPU;

class Debugger {
public:
    explicit Debugger(CPU *c, PPU *ppu);

    ~Debugger();

    void update();

    void reset();

    void dissassamble();

    void render() const;

    void step_mode();

private:
    CPU *cpu;
    PPU *ppu;

    Debug_State debug_state;
    SDL_Event event;

    bool next_step;
private:

    const void *frame_buffer = nullptr;
    const void *tiles_buffer = nullptr;

    GLuint frame_texture = 0;
    GLuint tiles_texture = 0;


    std::vector<debug_instruction> dis_vec;

    void init_debug_window();

    static void view_one();

    void imgui_demo();

    void CPU_view();

    void video_view();


    void terminate_window();

    static void glfw_error_callback(int error, const char *description);


    GLFWwindow *window{};
    bool show_demo_window = true;
    bool show_another_window = false;

    const ImVec4 bg_color = {0.8f, 0.8f, 0.8f, 1.0f};

    int window_height = 900;
    int window_width = 1200;

    int window_xpos = 680;
    int window_ypos = 45;

    static bool load_texture(GLuint *textureHandler, int width, int height, const void *data);

    void update_views();

    void control_view();
};


#endif //CPP_GB_DEBUGGER_H
