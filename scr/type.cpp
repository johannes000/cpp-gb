#include "type.h"

void set_bit_if(u8 &value, const u8 &bit, bool condition) {
	condition ? set_bit(value, bit) : reset_bit(value, bit);
}

void set_bit_if(u16 &value, const u16 &bit, bool condition) {
	condition ? set_bit(value, bit) : reset_bit(value, bit);
}

inline void set_bit(u8 &value, const u8 &bit) { value |= (1u << bit); }

inline void set_bit(u16 &value, const u16 &bit) { value |= (1u << bit); }

inline void reset_bit(u8 &value, const u8 &bit) { value &= ~(1u << bit); }

inline void reset_bit(u16 &value, const u16 &bit) { value &= ~(1u << bit); }

void toggle_bit(u8 &value, const u8 &bit) { value ^= (1u << bit); }

void toggle_bit(u16 &value, const u16 &bit) { value ^= (1u << bit); }

bool check_bit(const u8 value, const u8 bit) { return static_cast<u8>(value >> bit) & 1u; }

bool check_bit(const u16 value, const u16 bit) { return static_cast<u16>(value >> bit) & 1u; }

const char *Rom_Info::CGBFlag_string() {
	switch (this->cgb) {
		case CGBFlag::ON:
			return "ON";
		case CGBFlag::OFF:
			return "OFF";
	}
	return nullptr;
}

const char *Rom_Info::CType_string() const {
	switch (this->ctype) {
		case CType::ROM_ONLY:
			return "ROM ONLY";
		default:
			return "WRONG CTYPE";
		case CType::MBC1:
			return "MBC 1";
		case CType::MBC1_RAM:
			return "MBC 1 + Ram";
		case CType::MBC1_RAM_BATTERY:
			return "MBC 1 + RAM + Battery";
		case CType::MBC2:
			return "MBC 2";
		case CType::MBC2_BATTERY:
			return "MBC 2 + BATTERY";
		case CType::ROM_RAM:
			return "ROM RAM";
		case CType::ROM_RAM_BATTERY:
			return "Rom + Ram + Battery";
		case CType::MMM01:
			return "MMM01";
		case CType::MMM01_RAM:
			return "MMM01 + Ram";
		case CType::MMM01_RAM_BATTERY:
			return "MMM01 + Ram + Battery";
		case CType::MBC3_TIMER_BATTERY:
			return "MBC 3 + Timer + Battery";
		case CType::MBC3_TIMER_RAM_BATTERY:
			return "MBC 3 + Timer + Ram + Battery";
		case CType::MBC3:
			return "MBC 3";
		case CType::MBC3_RAM:
			return "MBC 3 + Ram";
		case CType::MBC3_RAM_BATTERY:
			return "MBC 3 + Ram + Battery";
		case CType::MBC5:
			return "MBC 5";
		case CType::MBC5_RAM:
			return "MBC 5 + Ram";
		case CType::MBC5_RAM_BATTERY:
			return "MBC 5 + Ram + Battery";
		case CType::MBC5_RUMBLE:
			return "MBC 5 + Rumble";
		case CType::MBC5_RUMBLE_RAM:
			return "MBC 5 + Rumble + Ram";
		case CType::MBC5_RUMBLE_RAM_BATTERY:
			return "MBC 5 + Rumble + Ram + Battery";
		case CType::MBC6:
			return "MBC 6";
		case CType::POCKET_CAMERA:
			return "Pocket Camera";
		case CType::BANDAI_TAMA5:
			return "Bandai Tama 5";
		case CType::HuC3:
			return "HuC 3";
		case CType::HUC1_RAM_BATTERY:
			return "HUC 1 + Ram + Battery";
	}
}

const char *Rom_Info::rom_size_string() const {
	switch (this->rom_size) {
		default:
			return "Wrong size";
		case ROMSize::KB32:
			return "32 KB";
		case ROMSize::KB64:
			return "64 KB";
		case ROMSize::KB128:
			return "128 KB";
		case ROMSize::KB256:
			return "256 KB";
		case ROMSize::KB512:
			return "512 KB";
		case ROMSize::MB1:
			return "1 MB";
		case ROMSize::MB2:
			return "2 MB";
		case ROMSize::MB4:
			return "4 MB";
		case ROMSize::MB8:
			return "8 MB";
		case ROMSize::MB1_1:
			return "1.1 MB";
		case ROMSize::MB1_2:
			return "1.2 MB";
		case ROMSize::MB1_5:
			return "1.5 MB";
	}
}

const char *Rom_Info::ram_size_string() const {
	switch (this->ram_size) {
		case RAMSize::None:
			return "No Ram";
		case RAMSize::KB2:
			return "2 KB";
		case RAMSize::KB8:
			return "8 KB";
		case RAMSize::KB32:
			return "32 KB";
		case RAMSize::KB128:
			return "128 KB";
		case RAMSize::KB64:
			return "64 KB";
		default:
			return "Wrong Ram Size";
	}
}
