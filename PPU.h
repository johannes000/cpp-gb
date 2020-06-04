#pragma once

#include <SDL.h>
#include <stdio.h>

class PPU {
public:
	bool createWindow();
	void renderScreen();

private:
	//Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Window* window;
	SDL_Surface* screenSurface;

};

