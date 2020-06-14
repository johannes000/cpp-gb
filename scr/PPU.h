#pragma once
#ifndef PPU_H_GUARD
#define PPU_H_GUARD

#include "type.h"
#include "MEMORY.h"

#include <SDL.h>
#include <stdio.h>
#include <vector>

constexpr u16 VRAM_SIZE = 0x1FFF;
constexpr u16 OAM_SIZE = 0x009F;

class MEMORY;

class PPU {
public:
	PPU(MEMORY* m);
	~PPU();
	void waitforinput();

	void ppu_write(const u16& addr, const u8& data);
	u8 ppu_read(const u16& addr);
private:
	MEMORY* mem;

	bool createWindow();
	void renderScreen();
	void closeWindow();

	SDL_Event event = event;
	// Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	std::vector<u8> v_ram;
	std::vector<u8> sprite_attribute_table;

};

#endif // !PPU_H_GUARD
