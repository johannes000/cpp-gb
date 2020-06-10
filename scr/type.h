#pragma once
#ifndef TYPE_H_GUARD
#define TYPE_H_GUARD

#include <cstdint>

typedef unsigned char u8;
typedef unsigned short u16;

typedef signed char i8;
typedef signed short i16;

const enum class Flag {
	z = 7, // Zero Flag
	n = 6, // Add/Sub-Flag 
	h = 5, // Half Carry Flag
	c = 4, // Carry Flag
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
	S32KByte = 0x00, //(no ROM banking)
	S64KByte = 0x01, //(4 banks)
	S128KByte = 0x02, //(8 banks)
	S256KByte = 0x03, //(16 banks)
	S512KByte = 0x04, //(32 banks)
	S1MByte = 0x05, //(64 banks) - only 63 banks used by MBC1
	S2MByte = 0x06, //(128 banks) - only 125 banks used by MBC1
	S4MByte = 0x07, //(256 banks)
	S8MByte = 0x08, //(512 banks)
	S1_1MByte = 0x52, //(72 banks)
	S1_2MByte = 0x53, //(80 banks)
	S1_5MByte = 0x54, //(96 banks)
};

const enum class RAMSize {
	None = 0x00,
	S2KB = 0x01,
	S8KB = 0x02,
	S32KB = 0x03, //(4 banks of 8KBytes each)
	S128KB = 0x04, //(16 banks of 8KBytes each)
	S64KB = 0x05, // (8 banks of 8KBytes each)
};

const enum class DestCode {
	JAP = 0x00,
	NON_JAP = 0x01,
};
#endif // !TYPE_H_GUARD

