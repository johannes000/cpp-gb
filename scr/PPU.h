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

	SDL_Window* window = nullptr;
	SDL_Surface* screenSurface = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;

	bool createWindow();
	void renderScreen();
	void closeWindow();

	SDL_Event event = event;
	// Screen dimension constants

	const int SCREEN_WIDTH = GB_LCD_SIZE_WIDTH;
	const int SCREEN_HEIGHT = GB_LCD_SIZE_HEIGHT;

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
		void update(u8 data);
	}lcd_control;

	enum class LCD_Status {
		lyc = 6,				// Bit 6 - LYC = LY Coincidence Interrupt(1 = Enable) (Read / Write)
		oam_interupt = 5,		// Bit 5 - Mode 2 OAM Interrupt(1 = Enable) (Read / Write)
		v_blank_interupt = 4,	// Bit 4 - Mode 1 V - Blank Interrupt(1 = Enable) (Read / Write)
		h_blank_interupt = 3,	// Bit 3 - Mode 0 H - Blank Interrupt(1 = Enable) (Read / Write)
		coincidence_flag = 2,	// Bit 2 - Coincidence Flag(0:LYC<>LY, 1 : LYC = LY) (Read Only)
		mode_flag_1 = 1,		// Bit 1 - 0 - Mode Flag(Mode 0 - 3, see below) (Read Only)
		mode_flag_0 = 0			// 0: During H - Blank
								// 1 : During V - Blank
								// 2 : During Searching OAM
								// 3 : During Transferring Data to LCD Driver
	}lcd_status;

	struct Pixel {
		// SDL_PIXELFORMAT_RGB24
		// Stored in memory as the bytes R, G, B
		// (R at the lowest address, B at the highest address).
		u8 R;
		u8 G;
		u8 B;
	};

	Pixel gb_screen[GB_LCD_SIZE_HEIGHT * GB_LCD_SIZE_WIDTH];

	std::vector<std::vector<Pixel>> Palettes =
		std::vector<std::vector<Pixel>>{
		{{0xE0,0xF8,0xD0},{0x88,0xC0,0x70},{0x34,0x68,0x56},{0x08,0x18,0x20}}, // classic grün
		{{175,203,70},{121,170,109},{34,111,95},{8,41,85}}, // Forum
		{{0xDF,0xDF,0xDF},{0xA9,0xA9,0xA9},{0x54,0x54,0x54},{0x00,0x0,0x0}}, // schwarz weiss
		{{255,255,255},{192,192,192},{96,96,96},{0x0,0x0,0x0}}, // schwarz weiss
	};
	u8 sel_pal = 0;
	void render_tiles();

	void change_palette();

};

#endif // !PPU_H_GUARD
