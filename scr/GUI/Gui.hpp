#ifndef GUI_HPP
#define GUI_HPP

#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

class gameboy;
struct Context {
	SDL_Window *window;
	SDL_GLContext gl_context;
};

struct TextureData {
	GLuint textureID;
	int width;
	int height;
};

class Gui {
public:
	Gui();
	void render(gameboy &gb);
	void destroy();

	Context *getContext() { return &context; }

private:
	Context context{};
	void showFPS() const;

	TextureData gb_framebuffer_texture;

public:
};

#endif