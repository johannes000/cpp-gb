//
// Created by knaub on 26.06.2020.
//

#include "Debugger.h"
#include "imgui/imgui_impl_glfw.h"

void Debugger::start_debugger() {

    glfwInit();

    std::string gsls_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, programName.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "[ERROR] Couldn't create a GLFW window\n";
        teardown(nullptr);
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(gsls_version.c_str());

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "[ERROR] Couldn't initialize GLAD" << std::endl;
        teardown(window);
        exit(-1);
    } else {
        std::cout << "[INFO] GLAD initialized\n";
    }
    ImGui_ImplGlfw_NewFrame();

    glClearColor(backgroundR, backgroundG, backgroundB, 1.0f);


}

void Debugger::teardown(GLFWwindow *window) {
    if (window != nullptr) { glfwDestroyWindow(window); }
    glfwTerminate();
}

Debugger::Debugger() {
    start_debugger();
}

void Debugger::update() {
    ImGui::Begin("The Plan");
    render();
}


void Debugger::render() {
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(backgroundR, backgroundG, backgroundB, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui::Render();
    glfwSwapBuffers(window);
}



