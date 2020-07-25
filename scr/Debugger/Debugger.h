//
// Created by knaub on 26.06.2020.
//

#ifndef CPP_GB_DEBUGGER_H
#define CPP_GB_DEBUGGER_H

#include "../type.h"

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

#include "../CPU.h"
#include "../PPU.h"
#include "../MEMORY.h"
#include "../CONTROLS.h"
#include "../CARDRIDGE.h"


class CPU;

class PPU;

class MEMORY;

class CARDRIDGE;

class CONTROLS;

class Debugger {
public:
	explicit Debugger(CPU* c, PPU* p, MEMORY* m, CARDRIDGE* card, CONTROLS* joypad);

	~Debugger();

	void update();

	void reset();

	void disassemble();

	void handle_breakpoints();

private:
	CPU* cpu;
	PPU* ppu;
	MEMORY* mem;
	CARDRIDGE* cardridge;
	CONTROLS* joypad;

	Debug_State debug_state;
public:
	void setDebugState(Debug_State debugState) { debug_state = debugState; }

	void update_views();

private:
	bool next_step;

	void update_tile_buffer();

	const char* what_register_is_this(const u8& reg);

	const char* what_register_is_this(const u16& reg);

	std::vector<debug_instruction> dis_vec;
	std::vector<u16> dis_addr_vec;

	std::vector<u16> breakpoints;


	void card_view();

	void cpu_view();

	void memory_view();

	void control_view();

	static bool load_texture(GLuint* textureHandler, int width, int height, const void* data);


	const ImVec4 bg_color = { 0.8f, 0.8f, 0.8f, 1.0f };
	ImVec4 cyan = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 magenta = ImVec4(1.0f, 0.502f, 0.957f, 1.0f);
	ImVec4 yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 red = ImVec4(1.0f, 0.149f, 0.447f, 1.0f);
	ImVec4 green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 gray = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	ImVec4 dark_gray = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

};


#endif //CPP_GB_DEBUGGER_H
