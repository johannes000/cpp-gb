//
// Created by knaub on 26.06.2020.
//

#ifndef CPP_GB_DEBUGGER_H
#define CPP_GB_DEBUGGER_H
// C++
#include <string>
#include <iostream>
// GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


class Debugger {
public:
    Debugger();

    void update();

private:
    void start_debugger();

    void render();

    GLFWwindow *window{};

    static void teardown(GLFWwindow *window);

    std::string programName = "Debugger";
    int windowWidth = 1200;
    int windowHeight = 800;
    float backgroundR = 0.1f;
    float backgroundG = 0.3f;
    float backgroundB = 0.2f;

    void composeControlView();
};


#endif //CPP_GB_DEBUGGER_H
