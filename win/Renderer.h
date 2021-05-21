//
// Created by knaub on 15.07.2020.
//

#ifndef CPP_GB_RENDERER_H
#define CPP_GB_RENDERER_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <string>

#include <map>

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h> // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h> // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h> // Initialize with gladLoadGL()

#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE	   // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h> // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE		 // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h> // Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && \
	!defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

class Debugger;
class CONTROLS;
struct gameboy;
struct Texture {
	GLuint texture_handler;
	GLuint *texture_ptr = &texture_handler;
	int width;
	int height;
	Pixel_a const *data_ptr = nullptr;
};
struct Renderer {
	GLFWwindow *window{};

	explicit Renderer(gameboy *gb);

	void init_window();

	static void glfw_error_callback(int error, const char *description);

	void terminate_window() const;

	virtual ~Renderer();

	void render() const;

	void start_new_imgui_frame();
	void bind_framebuffer(GLuint *frame_buffer_handle, GLuint *texture_handle);
	bool load_texture(Texture);
	static bool load_texture(GLuint *textureHandler, int width, int height, const void *data);

	void update_texture(Texture, const void *);
	void update_texture(GLuint *textureHandler, int width, int height, const void *data);

	std::vector<Pixel_a> tile_buffer;
	Texture frame_texture;
	f32 frame_buffer_ratio = 1.0;

	Texture tiles_texture;

	std::vector<Texture> sprite_textures;
	s32 window_height = 900;
	s32 window_width = 1700;

	s32 window_xpos = 50;
	s32 window_ypos = 45;

	s32 menu_bar_height = {};

	const ImVec4 bg_color = {0.8f, 0.8f, 0.8f, 1.0f};
	ImVec4 cyan = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 magenta = ImVec4(1.0f, 0.502f, 0.957f, 1.0f);
	ImVec4 yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 red = ImVec4(1.0f, 0.149f, 0.447f, 1.0f);
	ImVec4 green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 gray = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	ImVec4 dark_gray = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

	bool show_debug_views = true;
	bool show_demo_window = false;
	bool show_another_window = false;
	bool in_debug_show_fps = true;
	bool in_debug_cpu_debuger = false;
	bool in_debug_small_lcd_window = false;
	bool in_debug_tile_buffer = false;
	bool in_debug_card_view = false;
	bool in_debug_memory_view = false;
	bool in_debug_controler_view = false;

	void *frame_buffer = nullptr;
	GLuint frame_buffer_id = 0;

	gameboy *gb;

	void update_views();

	void update_tile_buffer();

	void gameboy_lcd_window();

	void tile_buffer_view();

	void menu();

	void show_fps() const;

	std::map<std::string, std::string> roms;

	void main_view(std::string *file);
	void file_menu_bar(std::string *file);
	void debug_menu_bar();
	void palette_menu_bar();
	void emulator_menu_bar();
};

void key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scancode,
				  int action, [[maybe_unused]] int mods);

#endif // CPP_GB_RENDERER_H
