
#ifndef PPU_H_GUARD
#define PPU_H_GUARD

#include "MEMORY.hpp"
#include "CPU.hpp"

#include "utl/type.hpp"

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

struct LCD_Register {
	Register LCD_SCY_; // SCY - Scroll Y (R/W)
	Register LCD_SCX_; // SCX - Scroll X (R/W)
	Register LCD_LY_;  // LY - LCDC Y-Coordinate (R)
	Register LCD_LYC_; // LYC - LY Compare (R/W)
	Register LCD_WY_;  // WY - Window Y Position (R/W)
	Register LCD_WX_;  // WX - Window X Position minus 7 (R/W)

	Register BG_PALETTE_DATA_; // BGP - BG Palette Data (R/W)
	// - Non CGB Mode Only
	Register SPRITE_PALETTE_DATA_0_; // OBP0 - Object Palette 0 Data (R/W)
	// - Non CGB Mode Only
	Register SPRITE_PALETTE_DATA_1_; // OBP1 - Object Palette 1 Data (R/W)
									 // - Non CGB Mode Only
};

enum class LCD_Mode {
	H_Blank, // 0: During H - Blank
	V_Blank, // 1 : During V - Blank
	OAM,	 // 2 : OAM Access
	VRAM,	 // 3 : VRAM Access
};
struct Sprite {
	int id_;
	int y_pos_;		   // Specifies the sprites vertical position on the screen (Y - 16)
	int x_pos_;		   // Specifies the sprites horizontal position on the screen (X - 8)
	u8 tile_no_;	   // Specifies the sprites Tile Number (00-FF)
	bool bg_priority_; // OBJ-to-BG Priority (0=OBJ Above BG, 1=OBJ Behind BG color 1-3)
	bool y_flip_;	   // Y flip          (0=Normal, 1=Vertically mirrored)
	bool x_flip_;	   // X flip          (0=Normal, 1=Horizontally mirrored)
	bool palette_;	   // Palette number  **Non CGB Mode Only** (0=OBP0, 1=OBP1)
	std::vector<u8> tile_data_;
	u8 *tile_data_ptr_ = &tile_data_[0];
};

class MEMORY;

class CPU;

class PPU {
public:
	explicit PPU(MEMORY *m, CPU *c);

	auto update(const int, bool) -> bool;

	auto write(const u16 &addr, const u8 &data) -> void;

	auto read(const u16 &addr) const -> u8;

	auto reset() -> void;

	int current_palette_ = 0;

private:
	CPU *cpu_;
	MEMORY *mem_;

	int cycles_in_mode_;
	int cycles_in_vblank_;
	int vblank_line_;
	bool ready_to_render_;

	auto update_tile(const u16 &addr) -> void;

public:
	auto is_ready_to_render() -> bool;

	// Speicher
	std::vector<u8> v_ram_;
	std::vector<u8> sprite_attribute_table_;

	// LCD Data
	LCD_Mode lcd_mode_{};

	LCD_Register lcd_register_{};

	auto get_lcd_register() const -> const LCD_Register & { return lcd_register_; }
	union LCD_Control {
		u8 value_;
		struct {
			u8 bg_enabled_ : 1;		   // Bit 0 - BG Enabled(in DMG) (0 = Disabled, 1 = Enabled)
			u8 sprites_enabled_ : 1;   // Bit 1 - Sprites Enabled (0 = Disabled, 1 = Enabled)
			u8 sprite_size_ : 1;	   // Bit 2 - Sprite Size(0 = 8x8, 1 = 8x16)
			u8 bg_tile_map_ : 1;	   // Bit 3 - BG Tile Map (0 = 9800h - 9BFFh, 1 = 9C00h - 9FFFh)
			u8 bg_window_tileset_ : 1; // Bit 4 - BG & Window Tileset(0 = 8800h - 97FFh, 1 = 8000h - 8FFFh)
			u8 window_enable_ : 1;	   // Bit 5 - Window Enable(0 = Disabled, 1 = Enabled)
			u8 window_tile_map_ : 1;   // Bit 6 - Window Tile Map (0 = 9800h - 9BFFh, 1 = 9C00h - 9FFFh)
			u8 lcd_power_ : 1;		   // Bit 7 - LCD Power(0 = Off, 1 = On)
		};
	} lcd_control_{};
	union LCD_State {
		u8 value_;
		struct {
			u8 mode_flag_0_ : 1;	   // 0: During H - Blank
									   // 1 : During V - Blank
									   // 2 : During Searching OAM
									   // 3 : During Transferring Data to LCD Driver
			u8 mode_flag_1_ : 1;	   // Bit 1 - 0 - Mode Flag (Mode 0 - 3, see below) (Read Only)
			u8 coincidence_flag_ : 1;  // Bit 2 - Coincidence Flag (0:LYC<>LY, 1 : LYC = LY) (Read Only)
			u8 h_blank_interrupt_ : 1; // Bit 3 - Mode 0 H - Blank Interrupt (1 = Enable) (Read / Write)
			u8 v_blank_interrupt_ : 1; // Bit 4 - Mode 1 V - Blank Interrupt (1 = Enable) (Read / Write)
			u8 oam_interrupt_ : 1;	   // Bit 5 - Mode 2 OAM Interrupt (1 = Enable) (Read / Write)
			u8 lyc_interrupt_ : 1;	   // Bit 6 - LYC = LY Coincidence Interrupt (1 = Enable) (Read /Write)
			u8 stub_ : 1;			   // Bit 7 - stub
		};
	} lcd_state_{};
	auto set_lcd_mode(LCD_Mode mode) -> void;

	auto update_lcd_mode() -> void;
	auto compare_ly_lyc() -> void;
	// LCD Render Funktionen
	std::vector<u8> palette_data_buffer_;
	std::vector<Pixel_a> bg_buffer_;
	std::vector<Pixel_a> sprite_buffer_;

	std::vector<std::vector<u8>> tile_map_data_;

	auto buffer_bg_line(const u8 line) -> std::vector<u8>;
	auto buffer_window_line(const u8 line) -> std::vector<u8>;
	auto buffer_sprite_line(const u8 line, std::vector<u8> *window_line) -> void;
	auto buffer_line(const u8 line) -> void;
	// Paletten
	std::vector<Pixel_a> bg_palette_;
	std::vector<std::vector<Pixel_a>> sprite_palette_;

	auto build_palette(const u16 addr, u8 data) -> void;

	// Sprites

	auto build_sprite_data(const u16 &addr, const u8 &data) -> void;

	std::vector<Sprite> sprites_;
	auto get_lcd_buffer() const -> Pixel_a const *;

	std::vector<std::vector<Pixel_a>> Palettes_ =
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
