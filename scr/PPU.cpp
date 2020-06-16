#include "PPU.h"
#include <cassert>
#include <cstdio>

PPU::PPU(MEMORY* m) :mem(m) {

	createWindow();
	v_ram = std::vector<u8>(VRAM_SIZE, 0x00);
	sprite_attribute_table = std::vector<u8>(OAM_SIZE, 0x00);
	memset(gb_screen, 0xFF, sizeof(gb_screen));
	lcd_control.update(read(LCD_CONTROL));

	std::copy(std::begin(palette_classic_gruen), std::end(palette_classic_gruen), std::begin(palette_forum));

	Uint32 fps_lasttime = SDL_GetTicks(); //the last recorded time.


	renderScreen();
}



PPU::~PPU() {
	closeWindow();
}
void PPU::update() {

	fps_frames++;
	if (fps_lasttime < SDL_GetTicks() - FPS_INTERVAL * 1000) {
		fps_lasttime = SDL_GetTicks();
		fps_current = fps_frames;
		fps_frames = 0;
		sprintf_s(fps_text, "%d", fps_current);
		render_tiles();
		SDL_SetWindowTitle(window, fps_text);
	}

	lcd_control.update(read(LCD_CONTROL));
	renderScreen();
}


void PPU::waitforinput() {
	bool quit = false;

	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			exit(0);
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_p:
					std::cout << "P" << std::endl;
					render_tiles();
					renderScreen();
					break;
				case SDLK_ESCAPE:
					exit(0);
					break;
				default:
					quit = true;
					break;
			}
			break;
		default:
			break;
	}

}


void PPU::write(const u16& addr, const u8& data) {
	if (addr >= GRAPHICS_RAM
		&& addr <= GRAPHICS_RAM_END) {
		v_ram[addr - GRAPHICS_RAM] = data;
	}
	else if (addr >= SPRITE_INFORMATION
			 && addr <= SPRITE_INFORMATION_END) {
		sprite_attribute_table[addr - SPRITE_INFORMATION] = data;
	}
	else {
		mem->write(addr, data);
	}
};


u8 PPU::read(const u16& addr) {
	if (addr >= GRAPHICS_RAM
		&& addr <= GRAPHICS_RAM_END) {
		return v_ram.at(addr - GRAPHICS_RAM);
	}
	else {
		return mem->read(addr);
	}
}

void PPU::closeWindow() {
	// Destroy window
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	// Quit SDL subsystems
	SDL_Quit();
}


void PPU::render_tiles() {
	int c = 0;
	int s = GB_LCD_SIZE_HEIGHT * GB_LCD_SIZE_WIDTH;

	//for (u16 i = 0; i < 2; i++) {
	//	gb_screen[c % s].R = current_palette[3].R;
	//	gb_screen[c % s].G = current_palette[3].G;
	//	gb_screen[c++ % s].B = current_palette[3].B;
	//}
	for (u16 i = 0x104; i < 0x9FFF; i += 2) {
		u8 data1 = read(i);
		u8 data2 = read(i + 1);

		for (size_t j = 0; j < 8; j++) {
			u8 p1 = ((data1 >> j) & 1) + (((data2 >> j) & 1));
			gb_screen[c % s].R = current_palette[p1].R;
			gb_screen[c % s].G = current_palette[p1].G;
			gb_screen[c % s].B = current_palette[p1].B;
			c++;
			if (c == s - 1)
				break;
		}

	}
}


bool PPU::createWindow() {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("cpp-gb",
							  SDL_WINDOWPOS_UNDEFINED,
							  SDL_WINDOWPOS_UNDEFINED,
							  SCREEN_WIDTH * 4,
							  SCREEN_HEIGHT * 4,
							  SDL_WINDOW_SHOWN);
	// Create renderer for window
	renderer = SDL_CreateRenderer(window, -1,
								  SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer,
								SDL_PIXELFORMAT_RGB24,
								SDL_TEXTUREACCESS_STREAMING,
								SCREEN_WIDTH, SCREEN_HEIGHT);

	return true;
}

void PPU::renderScreen() {
	assert(renderer && texture);
	SDL_UpdateTexture(texture, NULL,
					  gb_screen,
					  GB_LCD_SIZE_WIDTH * sizeof(u8) * 3);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void PPU::LCD_Control::update(u8 data) {
	*this = {
		(bool)((data >> 7) & 1),
		(bool)((data >> 6) & 1),
		(bool)((data >> 5) & 1),
		(bool)((data >> 4) & 1),
		(bool)((data >> 3) & 1),
		(bool)((data >> 2) & 1),
		(bool)((data >> 1) & 1),
		(bool)((data >> 0) & 1)
	};
}
