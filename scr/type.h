#pragma once
#ifndef TYPE_H_GUARD
#define TYPE_H_GUARD

#include <cstdint>
#include <stdexcept>

typedef unsigned char u8;
typedef unsigned short u16;

typedef signed char i8;
typedef signed short i16;

//ADDRESSES
constexpr u16 A_interrupt_flags = 0xFF0F;
constexpr u16 A_interrupt_enable = 0xFFFF;

typedef const u8 IF; // Interrupt Flags

const enum class Flag {
	z = 7, // Zero Flag
	n = 6, // Add/Sub-Flag 
	h = 5, // Half Carry Flag
	c = 4 // Carry Flag
};

struct Register {
	struct {
		union {
			struct {
				u8 F;
				u8 A;
			};
			u16 AF;
		};
	};

	struct {
		union {
			struct {
				u8 C;
				u8 B;
			};
			u16 BC;
		};
	};

	struct {
		union {
			struct {
				u8 E;
				u8 D;
			};
			u16 DE;
		};
	};

	struct {
		union {
			struct {
				u8 L;
				u8 H;
			};
			u16 HL;
		};
	};

	u16 SP;
	u16 PC;
};

//ROM INFO
const enum class CGBFlag {
	OFF = 0x80, // - Game supports CGB functions, but works on old gameboys also.
	ON = 0xC0, // - Game works on CGB only(physically the same as 80h).
};

const enum class CType { // Cartridge Type
	ROM_ONLY = 0x00,
	MBC1 = 0x01,
	MBC1_RAM = 0x02,
	MBC1_RAM_BATTERY = 0x03,
	MBC2 = 0x05,
	MBC2_BATTERY = 0x06,
	ROM_RAM = 0x08,
	ROM_RAM_BATTERY = 0x09,
	MMM01 = 0x0B,
	MMM01_RAM = 0x0C,
	MMM01_RAM_BATTERY = 0x0D,
	MBC3_TIMER_BATTERY = 0x0F,
	MBC3_TIMER_RAM_BATTERY = 0x10,
	MBC3 = 0x11,
	MBC3_RAM = 0x12,
	MBC3_RAM_BATTERY = 0x13,
	MBC5 = 0x19,
	MBC5_RAM = 0x1A,
	MBC5_RAM_BATTERY = 0x1B,
	MBC5_RUMBLE = 0x1C,
	MBC5_RUMBLE_RAM = 0x1D,
	MBC5_RUMBLE_RAM_BATTERY = 0x1E,
	MBC6 = 0x20,
	MBC7_SENSOR_RUMBLE_RAM_BATTERY = 022,
	POCKET_CAMERA = 0xFC,
	BANDAI_TAMA5 = 0xFD,
	HuC3 = 0xFE,
	HUC1_RAM_BATTERY = 0xFF


};

const enum class ROMSize {
	KB32 = 0x00, //(no ROM banking)
	KB64 = 0x01, //(4 banks)
	KB128 = 0x02, //(8 banks)
	KB256 = 0x03, //(16 banks)
	KB512 = 0x04, //(32 banks)
	MB1 = 0x05, //(64 banks) - only 63 banks used by MBC1
	MB2 = 0x06, //(128 banks) - only 125 banks used by MBC1
	MB4 = 0x07, //(256 banks)
	MB8 = 0x08, //(512 banks)
	MB1_1 = 0x52, //(72 banks)
	MB1_2 = 0x53, //(80 banks)
	MB1_5 = 0x54, //(96 banks)
};

const enum class RAMSize {
	None = 0x00,
	KB2 = 0x01,
	KB8 = 0x02,
	KB32 = 0x03, //(4 banks of 8KBytes each)
	KB128 = 0x04, //(16 banks of 8KBytes each)
	KB64 = 0x05, // (8 banks of 8KBytes each)
};

const enum class DestCode {
	JAP = 0x00,
	NON_JAP = 0x01,
};
#endif // !TYPE_H_GUARD

