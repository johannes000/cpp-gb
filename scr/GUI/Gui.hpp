#ifndef GUI_HPP
#define GUI_HPP

#pragma once

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL3/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#include "utl/type.hpp"

struct gameboy;
struct Context {
	SDL_Window *window_;
	SDL_GLContext gl_context_;
};

struct TextureData {
	GLuint textureID_;
	int width_;
	int height_;
};

class Gui {
public:
	Gui();
	auto render(gameboy &gb) -> void;
	auto destroy() -> void;

	auto getContext() -> Context * { return &context_; }

private:
	auto generate_widgets(gameboy &gb) -> void;

	Context context_{};
	auto showFPS() const -> void;

	TextureData gb_framebuffer_texture_;
	TextureData tile_buffer_texture_;
	std::vector<Pixel_a> tile_buffer_;

public:
};

#endif