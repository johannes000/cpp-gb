#pragma once
#ifndef PPU_H_GUARD
#define PPU_H_GUARD

#include <SDL.h>
#include <stdio.h>

class PPU {
public:
	bool createWindow();
	void renderScreen();
	void waitforinput();
	void closeWindow();

private:
	SDL_Event event = event;
	// Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
};

#endif // !PPU_H_GUARD