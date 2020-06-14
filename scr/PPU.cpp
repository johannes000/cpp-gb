#include "PPU.h"

PPU::PPU(MEMORY* m) : mem(m) {
	createWindow();
	renderScreen();

	v_ram = std::vector<u8>(VRAM_SIZE, 0x00);
}

PPU::~PPU() {
	closeWindow();
}

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
		if (SDL_WaitEvent(&event)) {
			/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
			switch (event.type) {
				case SDL_QUIT:
					exit(0);
					break;
				case SDL_KEYDOWN:
					quit = true;
					break;
				default:
					break;
			}
		}
	}
}

void PPU::ppu_write(const u16& addr, const u8& data) {
	if (addr >= GRAPHICS_RAM && addr <= GRAPHICS_RAM_END) {
		v_ram[addr - GRAPHICS_RAM] = data;
	}
	else {
		mem->write(addr, data);
	}
};


u8 PPU::ppu_read(const u16& addr) {
	if (addr >= GRAPHICS_RAM && addr <= GRAPHICS_RAM_END) {
		return v_ram.at(addr - GRAPHICS_RAM);
	}
	else {
		return mem->read(addr);
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
