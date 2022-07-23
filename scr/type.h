#ifndef TYPE_H_GUARD
#define TYPE_H_GUARD

#include <vector>
#include <ostream>
#include <iostream>
#include <cstdint>
#include <string>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef unsigned char Register;

// GAMEBOY STARTER MODES
constexpr u16 GB_SKIP_BOOTROM = 1 << 0;
constexpr u16 GB_NO_RENDER = 1 << 1;

// ADDRESSES
constexpr u16 INTERRUPT_FLAGS_REG = 0xFF0F;
constexpr u16 INTERRUPT_ENABLE_REG = 0xFFFF;

constexpr u16 CARD_ROM = 0x0000;
constexpr u16 CARD_ROM_0 = 0x0000;
constexpr u16 BOOT_ROM_END = 0x00FF;
constexpr u16 CARD_ROM_0_END = 0x3FFF;
constexpr u16 CARD_ROM_END = 0x7FFF;
constexpr u16 BIOS = 0x0000;
constexpr u16 BIOS_END = 0x00FF;
constexpr u16 CARD_HEADER = 0x0100;
constexpr u16 CARD_BANKS = 0x4000;
constexpr u16 CARD_BANKS_END = 0x7FFF;
constexpr u16 GRAPHICS_RAM = 0x8000;
constexpr u16 GRAPHICS_RAM_END = 0x9FFF;
constexpr u16 CARD_EXTERNAL_RAM = 0xA000;
constexpr u16 CARD_EXTERNAL_RAM_END = 0xBFFF;
constexpr u16 WORKING_RAM = 0xC000;
constexpr u16 WORKING_RAM_END = 0xDFFF;
constexpr u16 WORKING_RAM_SHADOW = 0xE000;
constexpr u16 WORKING_RAM_SHADOW_END = 0xFDFF;
constexpr u16 WORKING_RAM_SHADOW_OFFSET = 0x2000;
constexpr u16 SPRITE_INFORMATION = 0xFE00;
constexpr u16 SPRITE_INFORMATION_END = 0xFE9F;
constexpr u16 MEMORY_MAPPED_IO = 0xFF00;
constexpr u16 MEMORY_MAPPED_IO_END = 0xFF7F;
constexpr u16 ZERO_PAGE_RAM = 0xFF80;
constexpr u16 ZERO_PAGE_RAM_END = 0xFFFE;
constexpr u16 IN_BOOT_ADDRESS = 0xFF50;

constexpr int TILES_IN_VRAM = 384;

// LCD
constexpr int GB_LCD_SIZE_WIDTH = 160;
constexpr int GB_LCD_SIZE_HEIGHT = 144;

constexpr unsigned int TILE_WIDTH = 8;
constexpr unsigned int TILE_HEIGHT = 8;
constexpr unsigned int TILE_PER_LINE = 32;
constexpr unsigned int BG_MAP_SIZE = 256;
constexpr unsigned int TILE_MAP_WIDTH = 128;
constexpr unsigned int TILE_MAP_HEIGHT = 192;

// LCD Register Adressen
constexpr u16 LCD_CONTROL = 0xFF40;
constexpr u16 LCD_STATUS = 0xFF41;

constexpr u16 LCD_SCY = 0xFF42; // SCY - Scroll Y (R/W)
constexpr u16 LCD_SCX = 0xFF43; // SCX - Scroll X (R/W)
constexpr u16 LCD_LY = 0xFF44;	// LY - LCDC Y-Coordinate (R)
constexpr u16 LCD_LYC = 0xFF45; // LYC - LY Compare (R/W)
constexpr u16 LCD_WY = 0xFF4A;	// WY - Window Y Position (R/W)
constexpr u16 LCD_WX = 0xFF4B;	// WX - Window X Position minus 7 (R/W)

constexpr u16 BG_PALETTE_DATA =
	0xFF47; // BGP - BG Palette Data (R/W) - Non CGB Mode Only
constexpr u16 SPRITE_PALETTE_DATA_0 =
	0xFF48; // OBP0 - Object Palette 0 Data (R/W) - Non CGB Mode Only
constexpr u16 SPRITE_PALETTE_DATA_1 =
	0xFF49; // OBP1 - Object Palette 1 Data (R/W) - Non CGB Mode Only

// JOYPAD REGISTER
constexpr u16 JOY_P1_REGISTER = 0xFF00;

// TIMER REGISTER ADRESSEN
constexpr u16 DIVIDER_REGISTER = 0xFF04;
constexpr u16 TIMER_COUNTER = 0xFF05;
constexpr u16 TIMER_MODULO = 0xFF06;
constexpr u16 TIMER_CONTROL = 0xFF07;
constexpr int OSCILLATOR_CLOCK_FREQ = 4194304;

constexpr u16 DMA_TRANSFER = 0xFF46; // DMA Transfer and Start Address (R/W)

// APU ADRESSEN
// Sound Channel 1 - Tone & Sweep
constexpr u16 NR10 = 0xFF10; // FF10 - NR10 - Channel 1 Sweep register (R/W)
constexpr u16 NR11 = 0xFF11; // FF11 - NR11 - Channel 1 Sound length/Wave pattern duty (R/W)
constexpr u16 NR12 = 0xFF12; // FF12 - NR12 - Channel 1 Volume Envelope (R/W)
constexpr u16 NR13 = 0xFF13; // FF13 - NR13 - Channel 1 Frequency lo (Write Only)
constexpr u16 NR14 = 0xFF14; // FF14 - NR14 - Channel 1 Frequency hi (R/W)
// Sound Channel 2 - Tone
constexpr u16 NR21 = 0xFF16; // FF16 - NR21 - Channel 2 Sound Length/Wave Pattern Duty (R/W)
constexpr u16 NR22 = 0xFF17; // FF17 - NR22 - Channel 2 Volume Envelope (R/W)
constexpr u16 NR23 = 0xFF18; // FF18 - NR23 - Channel 2 Frequency lo data (W)
constexpr u16 NR24 = 0xFF19; // FF19 - NR24 - Channel 2 Frequency hi data (R/W)
// Sound Channel 3 - Wave Output
constexpr u16 NR30 = 0xFF1A; // FF1A - NR30 - Channel 3 Sound on/off (R/W)
constexpr u16 NR31 = 0xFF1B; // FF1B - NR31 - Channel 3 Sound Length (W)
constexpr u16 NR32 = 0xFF1C; // FF1C - NR32 - Channel 3 Select output level (R/W)
constexpr u16 NR33 = 0xFF1D; // FF1D - NR33 - Channel 3 Frequency’s lower data (W)
constexpr u16 NR34 = 0xFF1E; // FF1E - NR34 - Channel 3 Frequency’s higher data (R/W)
// Sound Channel 4 - Noise
constexpr u16 NR41 = 0xFF20; // FF20 - NR41 - Channel 4 Sound Length (W)
constexpr u16 NR42 = 0xFF21; // FF21 - NR42 - Channel 4 Volume Envelope (R/W)
constexpr u16 NR43 = 0xFF22; // FF22 - NR43 - Channel 4 Polynomial Counter (R/W)
constexpr u16 NR44 = 0xFF23; // FF23 - NR44 - Channel 4 Counter/consecutive; Inital (R/W)
constexpr u16 NR50 = 0xFF24; // FF24 - NR50 - Channel control / ON-OFF / Volume (R/W)
constexpr u16 NR51 = 0xFF25; // FF25 - NR51 - Selection of Sound output terminal (R/W)
constexpr u16 NR52 = 0xFF26; // FF26 - NR52 - Sound on/off
// Wave Table
constexpr u16 WAVE_TABLE_START = 0xFF30; // 0xFF30 - Samples 0 and 1
constexpr u16 WAVE_TABLE_END = 0xFF3F;	 // 0xFF3F - Samples 30 and 31

constexpr int VERTICAL_REFRESH_CYCLES = 70224;

enum class Interrupt {
	V_Blank = 0,
	LCD_Stat,
	Timer_overflow,
	Serial_link,
	Joypad,
};
struct debug_instruction {
	const char *mnemonic{};
	u8 bytes{};
	u8 cycles{};
	u8 opcode{};
	u16 intermediate{};
	u16 address{};
	bool prefixed{false};
};
struct Pixel {
	// GL_RGB
	// Red, Green, Blue
	u8 R;
	u8 G;
	u8 B;
};
struct Pixel_a {
	// GL_RGBA
	// Red, Green, Blue, Alpha
	u8 R;
	u8 G;
	u8 B;
	u8 A;
	inline bool operator==(Pixel_a Comp) {
		return (Comp.R == this->R && Comp.G == this->G && Comp.B == this->B && Comp.A == this->A);
	}
};

inline void set_bit(u8 &value, const u8 &bit) {
	value |= (1u << bit);
}

inline void set_bit(u16 &value, const u16 &bit) {
	value |= (1u << bit);
}

inline void reset_bit(u8 &value, const u8 &bit) {
	value &= ~(1u << bit);
}

inline void reset_bit(u16 &value, const u16 &bit) {
	value &= ~(1u << bit);
}

inline void toggle_bit(u8 &value, const u8 &bit) {
	value ^= (1u << bit);
}

inline void toggle_bit(u16 &value, const u16 &bit) {
	value ^= (1u << bit);
}

inline bool check_bit(const u8 value, const u8 bit) {
	return static_cast<u8>(value >> bit) & 1u;
}

inline bool check_bit(const u16 value, const u16 bit) {
	return static_cast<u16>(value >> bit) & 1u;
}

inline void set_bit_if(u8 &value, const u8 &bit, bool condition) {
	condition ? set_bit(value, bit) : reset_bit(value, bit);
}

inline void set_bit_if(u16 &value, const u16 &bit, bool condition) {
	condition ? set_bit(value, bit) : reset_bit(value, bit);
}

#endif // !TYPE_H_GUARD
