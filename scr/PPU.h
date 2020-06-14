#pragma once
#ifndef PPU_H_GUARD
#define PPU_H_GUARD

#include "type.h"
#include "MEMORY.h"

#include <SDL.h>
#include <stdio.h>
#include <vector>

constexpr u16 VRAM_SIZE = 0x2000;
constexpr u16 OAM_SIZE = 0x0100;


class MEMORY;

class PPU {
public:
	PPU(MEMORY* m);
	~PPU();
	void waitforinput();
	void update();

	void write(const u16& addr, const u8& data);
	u8 read(const u16& addr);
private:
	MEMORY* mem;

	bool createWindow();
	void renderScreen();
	void closeWindow();

	SDL_Event event = event;
	// Screen dimension constants

	const int SCREEN_WIDTH = GB_LCD_SIZE_WIDTH;
	const int SCREEN_HEIGHT = GB_LCD_SIZE_HEIGHT;
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	std::vector<u8> v_ram;
	std::vector<u8> sprite_attribute_table;

	struct LCD_Control {
		bool lcd_power;				// Bit 7 - LCD Power(0 = Off, 1 = On)
		bool window_tile_map;		// Bit 6 - Window Tile Map(0 = 9800h - 9BFFh, 1 = 9C00h - 9FFFh)
		bool window_enable;			// Bit 5 - Window Enable(0 = Disabled, 1 = Enabled)
		bool bg_window_tileset;		// Bit 4 - BG & Window Tileset(0 = 8800h - 97FFh, 1 = 8000h - 8FFFh)
		bool bg_tile_map;			// Bit 3 - BG Tile Map(0 = 9800h - 9BFFh, 1 = 9C00h - 9FFFh)
		bool sprite_size;			// Bit 2 - Sprite Size(0 = 8×8, 1 = 8×16)
		bool sprites_enabled;		// Bit 1 - Sprites Enabled(0 = Disabled, 1 = Enabled)
		bool bg_enabled;			// Bit 0 - BG Enabled(in DMG) (0 = Disabled, 1 = Enabled)
	}lcd_control;
	void update_lcd_control();

	enum class LCD_Status {
		lyc = 6,				// Bit 6 - LYC = LY Coincidence Interrupt(1 = Enable) (Read / Write)
		oam_inter = 5,			// Bit 5 - Mode 2 OAM Interrupt(1 = Enable) (Read / Write)
		v_blank_inter = 4,		// Bit 4 - Mode 1 V - Blank Interrupt(1 = Enable) (Read / Write)
		h_blank_inter = 3,		// Bit 3 - Mode 0 H - Blank Interrupt(1 = Enable) (Read / Write)
		coincidence_flag = 2,	// Bit 2 - Coincidence Flag(0:LYC<>LY, 1 : LYC = LY) (Read Only)
		mode_flag_1 = 1,		// Bit 1 - 0 - Mode Flag(Mode 0 - 3, see below) (Read Only)
		mode_flag_0 = 0			// 0: During H - Blank
								// 1 : During V - Blank
								// 2 : During Searching OAM
								// 3 : During Transferring Data to LCD Driver
	}lcd_status;

	struct LCD {

	};
};

#endif // !PPU_H_GUARD
