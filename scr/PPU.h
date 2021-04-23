
#ifndef PPU_H_GUARD
#define PPU_H_GUARD

#include <vector>

#include "CPU.h"
#include "MEMORY.h"
#include "type.h"

constexpr auto FPS_INTERVAL = 1.0; // seconds.;

constexpr u16 VRAM_SIZE = 0x2000;
constexpr u16 OAM_SIZE = 0x0100;

constexpr int CLOCKS_SCANLINE_OAM = 80;
constexpr int CLOCKS_SCANLINE_VRAM = 172;
constexpr int CLOCKS_HORIZONTAL_BLANK = 204;
constexpr int CLOCKS_SCANLINE =
	CLOCKS_SCANLINE_OAM + CLOCKS_SCANLINE_VRAM + CLOCKS_HORIZONTAL_BLANK;
constexpr int CLOCKS_VERTICAL_BLANK = CLOCKS_SCANLINE * 10;
constexpr int CLOCKS_FULL_FRAME = 70224;

constexpr int VERTICAL_BLANK_LINES = 154;

class MEMORY;

class CPU;

class PPU {
public:
	explicit PPU(MEMORY *m, CPU *c);

	void update(const int, bool);

	void write(const u16 &addr, const u8 &data);

	u8 read(const u16 &addr) const;

	void reset();

	bool ready_to_render;

	int current_palette = 0;

private:
	CPU *cpu;
	MEMORY *mem;

	int cycles_in_mode{};

	void update_tile(const u16 &addr);
	void handle_lcd_status(const u8 &data);

public:
	inline bool
	is_ready_to_render() const { return ready_to_render; }

	// Speicher
private:
	std::vector<u8> v_ram;
	std::vector<u8> sprite_attribute_table;

	// LCD Data
	LCD_Mode lcd_mode{};

	LCD_Register lcd_register{};

public:
	const LCD_Register &get_lcd_register() const { return lcd_register; }

private:
	struct LCD_Control {
		bool lcd_power;			// Bit 7 - LCD Power(0 = Off, 1 = On)
		bool window_tile_map;	// Bit 6 - Window Tile Map (0 = 9800h - 9BFFh, 1 = 9C00h - 9FFFh)
		bool window_enable;		// Bit 5 - Window Enable(0 = Disabled, 1 = Enabled)
		bool bg_window_tileset; // Bit 4 - BG & Window Tileset(0 = 8800h - 97FFh, 1 = 8000h - 8FFFh)
		bool bg_tile_map;		// Bit 3 - BG Tile Map (0 = 9800h - 9BFFh, 1 = 9C00h - 9FFFh)
		bool sprite_size;		// Bit 2 - Sprite Size(0 = 8x8, 1 = 8x16)
		bool
			sprites_enabled; // Bit 1 - Sprites Enabled (0 = Disabled, 1 = Enabled)
		bool bg_enabled;	 // Bit 0 - BG Enabled(in DMG) (0 = Disabled, 1 = Enabled)
		void update(PPU *ppu);
	} lcd_control{};

	struct LCD_State {
		// Bit 6 - LYC = LY Coincidence Interrupt (1 = Enable) (Read /Write)
		bool lyc_interrupt;
		// Bit 5 - Mode 2 OAM Interrupt (1 = Enable) (Read / Write)
		bool oam_interrupt;
		// Bit 4 - Mode 1 V - Blank Interrupt (1 = Enable) (Read / Write)
		bool v_blank_interrupt;
		// Bit 3 - Mode 0 H - Blank Interrupt (1 = Enable) (Read / Write)
		bool h_blank_interrupt;
		// Bit 2 - Coincidence Flag (0:LYC<>LY, 1 : LYC = LY) (Read Only)
		bool coincidence_flag;
		// Bit 1 - 0 - Mode Flag (Mode 0 - 3, see below) (Read Only)
		bool mode_flag_1; // 0: During H - Blank
		bool mode_flag_0; // 1 : During V - Blank
						  // 2 : During Searching OAM
						  // 3 : During Transferring Data to LCD Driver

	} lcd_state{};

	void set_lcd_mode(LCD_Mode mode);

	void update_lcd_mode();

	// LCD Render Funktionen

	std::vector<u8> palette_data_buffer;
	std::vector<Pixel_a> bg_buffer;
	std::vector<Pixel_a> sprite_buffer;

public:
	std::vector<std::vector<u8>> tile_map_data;

private:
	void buffer_bg_line(const u8 line);
	void buffer_sprite_line(const u8 line);
	void buffer_window_line(const u8 line);

public:
	// Paletten
	std::vector<Pixel_a> bg_palette;
	std::vector<std::vector<Pixel_a>> sprite_obj_palette;

private:
	void build_palette(const u16 addr, u8 data);

	// Sprites

	struct Sprite {
		int id;
		int y_pos;		  // Specifies the sprites vertical position on the screen (Y - 16)
		int x_pos;		  // Specifies the sprites horizontal position on the screen (X - 8)
		u8 tile_no;		  // Specifies the sprites Tile Number (00-FF)
		bool bg_priority; // OBJ-to-BG Priority (0=OBJ Above BG, 1=OBJ Behind BG color 1-3)
		bool y_flip;	  // Y flip          (0=Normal, 1=Vertically mirrored)
		bool x_flip;	  // X flip          (0=Normal, 1=Horizontally mirrored)
		bool palette;	  // Palette number  **Non CGB Mode Only** (0=OBP0, 1=OBP1)
		std::vector<u8> tile_data;
		u8 *tile_data_ptr = &tile_data[0];
	};
	void build_sprite_data(const u16 &addr, const u8 &data);

public:
	std::vector<Sprite> sprites;
	Pixel_a const *get_lcd_buffer() const;

	std::vector<std::vector<Pixel_a>> Palettes =
		std::vector<std::vector<Pixel_a>>{
			{{0xE0, 0xF8, 0xD0, 0xFF},
			 {0x88, 0xC0, 0x70, 0xFF},
			 {0x34, 0x68, 0x56, 0xFF},
			 {0x08, 0x18, 0x20, 0xFF}}, // classic grün
			{{175, 203, 70, 255},
			 {121, 170, 109, 255},
			 {34, 111, 95, 255},
			 {8, 41, 85, 255}}, // Forum grün
			{{0xDF, 0xDF, 0xDF, 0xFF},
			 {0xA9, 0xA9, 0xA9, 0xFF},
			 {0x54, 0x54, 0x54, 0xFF},
			 {0x00, 0x0, 0x0, 0xFF}}, // schwarz weiss
			{{255, 255, 255, 255},
			 {192, 192, 192, 255},
			 {96, 96, 96, 255},
			 {0, 0, 0, 255}}, // schwarz weiss
		};
};

#endif // !PPU_H_GUARD
