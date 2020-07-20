//
// Created by knaub on 15.07.2020.
//

#ifndef CPP_GB_RENDERER_H
#define CPP_GB_RENDERER_H

#include "type.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <imfilebrowser.h>

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

#include "PPU.h"
#include "Debugger\Debugger.h"
#include "CONTROLS.h"

class CONTROLS;

class PPU;

class Debugger;

struct Renderer {
	GLFWwindow* window{};

	explicit Renderer(CONTROLS* joypad);

	void init_window();

	static void glfw_error_callback(int error, const char* description);

	void terminate_window() const;

	virtual ~Renderer();

	void render() const;

	static bool load_texture(GLuint* textureHandler, int width, int height, const void* data);

	int window_height = 900;
	int window_width = 1700;

	int window_xpos = 50;
	int window_ypos = 45;


	const ImVec4 bg_color = { 0.8f, 0.8f, 0.8f, 1.0f };
	ImVec4 cyan = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 magenta = ImVec4(1.0f, 0.502f, 0.957f, 1.0f);
	ImVec4 yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 red = ImVec4(1.0f, 0.149f, 0.447f, 1.0f);
	ImVec4 green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 gray = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	ImVec4 dark_gray = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);


	bool show_demo_window = true;
	bool show_another_window = false;

	void* frame_buffer = nullptr;
	GLuint frame_texture = 0;
	GLuint tiles_texture = 0;
	std::vector<Pixel> tile_buffer;

	ImGui::FileBrowser fileDialog;

	std::string file_browser();

	void update_views(PPU* ppu, Debugger* debug);

	void main_view(PPU* ppu);

	void show_fps() const;

	std::map<std::string, std::string> roms;

	CONTROLS* joypad;

};

void key_callback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods);

#endif //CPP_GB_RENDERER_H
