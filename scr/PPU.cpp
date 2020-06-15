#include "PPU.h"
#include <cassert>

PPU::PPU(MEMORY* m) : mem(m) {
	createWindow();
	v_ram = std::vector<u8>(VRAM_SIZE, 0x00);
	sprite_attribute_table = std::vector<u8>(OAM_SIZE, 0x00);

	memset(gb_screen, 0xFF, sizeof(gb_screen));

	renderScreen();
	update_lcd_control();
}

PPU::~PPU() {
	closeWindow();
}
void PPU::update() {
	update_lcd_control();
	render_tiles();
	renderScreen();
}


void PPU::waitforinput() {
	bool quit = false;
	while (!quit) {
		if (SDL_WaitEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					exit(0);
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_p:
							sel_pal = (++sel_pal) % Palettes.size();
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

void PPU::update_lcd_control() {
	u8 LCD_data = read(LCD_CONTROL);
	lcd_control = {
		(bool)((LCD_data >> 7) & 1),
		(bool)((LCD_data >> 6) & 1),
		(bool)((LCD_data >> 5) & 1),
		(bool)((LCD_data >> 4) & 1),
		(bool)((LCD_data >> 3) & 1),
		(bool)((LCD_data >> 2) & 1),
		(bool)((LCD_data >> 1) & 1),
		(bool)((LCD_data >> 0) & 1)
	};
}

void PPU::render_tiles() {
	int c = 0;
	for (auto& p : gb_screen) {
		u8 pp = mem->read((0x0104 + c++) % 0x2F);
		p.R = Palettes[sel_pal][(pp >> 2) & 3].R;
		p.G = Palettes[sel_pal][(pp >> 2) & 3].G;
		p.B = Palettes[sel_pal][(pp >> 2) & 3].B;
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

