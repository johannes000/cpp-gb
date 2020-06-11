#include "PPU.h"

void PPU::renderScreen() {
	screenSurface = SDL_GetWindowSurface(window);
	// Fill the surface white
	SDL_FillRect(screenSurface, NULL,
				 SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

	// Update the surface
	SDL_UpdateWindowSurface(window);

	// Wait two seconds
	SDL_Delay(20);

}

void PPU::waitforinput() {
	bool quit = false;
	while (!quit) {
		if (SDL_PollEvent(&event)) {
			/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
			switch (event.type) {
			case SDL_KEYDOWN:
				quit = true;
				break;
			default:
				break;
			}
		}
	}
}

void PPU::closeWindow() {
	// Destroy window
	SDL_DestroyWindow(window);

	// Quit SDL subsystems
	SDL_Quit();
}

bool PPU::createWindow() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		// Create window
		window = SDL_CreateWindow("Gameboy", 200, 200, SCREEN_WIDTH,
								  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n",
				   SDL_GetError());
			return false;
		}
	}

	return true;
}
