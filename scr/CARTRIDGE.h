#ifndef CARTRIDGE_H_GUARD
#define CARTRIDGE_H_GUARD

#include <vector>

#include "type.h"

constexpr u16 CARD_ROM_BANK_SIZE = 0x4000;
constexpr u16 CARD_RAM_BANK_SIZE = 0x4000;

// ROM INFO
enum class CGBFlag {
	OFF [[maybe_unused]] = 0x80, // - Game supports CGB functions, but works on old gameboys also.
	ON [[maybe_unused]] = 0xC0,	 // - Game works on CGB only(physically the same as 80h).
};
enum class RomRamBankSwitch {
	ROM,
	RAM,
	undefined
};

enum class CType { // Cartridge Type
	ROM_ONLY [[maybe_unused]] = 0x00,
	MBC1 [[maybe_unused]] = 0x01,
	MBC1_RAM [[maybe_unused]] = 0x02,
	MBC1_RAM_BATTERY [[maybe_unused]] = 0x03,
	MBC2 [[maybe_unused]] = 0x05,
	MBC2_BATTERY [[maybe_unused]] = 0x06,
	ROM_RAM [[maybe_unused]] = 0x08,
	ROM_RAM_BATTERY [[maybe_unused]] = 0x09,
	MMM01 [[maybe_unused]] = 0x0B,
	MMM01_RAM [[maybe_unused]] = 0x0C,
	MMM01_RAM_BATTERY [[maybe_unused]] = 0x0D,
	MBC3_TIMER_BATTERY [[maybe_unused]] = 0x0F,
	MBC3_TIMER_RAM_BATTERY [[maybe_unused]] = 0x10,
	MBC3 [[maybe_unused]] = 0x11,
	MBC3_RAM [[maybe_unused]] = 0x12,
	MBC3_RAM_BATTERY [[maybe_unused]] = 0x13,
	MBC5 [[maybe_unused]] = 0x19,
	MBC5_RAM [[maybe_unused]] = 0x1A,
	MBC5_RAM_BATTERY [[maybe_unused]] = 0x1B,
	MBC5_RUMBLE [[maybe_unused]] = 0x1C,
	MBC5_RUMBLE_RAM [[maybe_unused]] = 0x1D,
	MBC5_RUMBLE_RAM_BATTERY [[maybe_unused]] = 0x1E,
	MBC6 [[maybe_unused]] = 0x20,
	MBC7_SENSOR_RUMBLE_RAM_BATTERY [[maybe_unused]] = 0x22,
	POCKET_CAMERA [[maybe_unused]] = 0xFC,
	BANDAI_TAMA5 [[maybe_unused]] = 0xFD,
	HuC3 [[maybe_unused]] = 0xFE,
	HUC1_RAM_BATTERY [[maybe_unused]] = 0xFF
};

enum class ROMSize {
	KB32 = 0x00,  //(no ROM banking)
	KB64 = 0x01,  //(4 banks)
	KB128 = 0x02, //(8 banks)
	KB256 = 0x03, //(16 banks)
	KB512 = 0x04, //(32 banks)
	MB1 = 0x05,	  //(64 banks) - only 63 banks used by MBC1
	MB2 = 0x06,	  //(128 banks) - only 125 banks used by MBC1
	MB4 = 0x07,	  //(256 banks)
	MB8 = 0x08,	  //(512 banks)
	MB1_1 = 0x52, //(72 banks)
	MB1_2 = 0x53, //(80 banks)
	MB1_5 = 0x54, //(96 banks)
};

enum class RAMSize {
	None = 0x00,
	KB2 = 0x01,
	KB8 = 0x02,
	KB32 = 0x03,  //(4 banks of 8KBytes each)
	KB128 = 0x04, //(16 banks of 8KBytes each)
	KB64 = 0x05,  //(8 banks of 8KBytes each)
};

enum class DestCode {
	JAP [[maybe_unused]] = 0x00,
	NON_JAP [[maybe_unused]] = 0x01,
};

struct Rom_Info {

	std::string filename;
	std::string filepath;

	std::string savefile;

	char title[15] = "";
	bool SGBFlag = false;

	CGBFlag cgb = CGBFlag::ON;
	CType ctype = CType::MBC1;

	ROMSize rom_size = ROMSize::KB32;
	RAMSize ram_size = RAMSize::None;

	int rom_banks{};
	int ram_banks{};

public:
	inline const char *CGBFlag_string() {
		switch (this->cgb) {
			case CGBFlag::ON:
				return "ON";
			case CGBFlag::OFF:
				return "OFF";
		}
		return nullptr;
	}

	inline const char *CType_string() const {
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

	inline const char *rom_size_string() const {
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

	inline const char *ram_size_string() const {
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
};

class CARTRIDGE {
public:
	CARTRIDGE();
	bool rom_loaded = false;
	bool load_rom(std::string rom_file, bool skip_boot);

	Rom_Info rom_info;

	u8 read(const u16 &addr);
	u8 readBoot(const u16 &addr);
	u8 readNoMBC(const u16 &addr);
	u8 readMBC1(const u16 &addr);
	u8 readMBC2(const u16 &addr);
	u8 readMBC3(const u16 &addr);
	u8 readMBC5(const u16 &addr);
	u8 readMBC6(const u16 &addr);
	u8 readMBC7(const u16 &addr);
	u8 readHuC1(const u16 &addr);

	void write(const u16 addr, const u8 &data);
	void writeNoMBC(const u16 addr, const u8 &data);
	void writeMBC1(const u16 addr, const u8 &data);
	void writeMBC2(const u16 addr, const u8 &data);
	void writeMBC3(const u16 addr, const u8 &data);
	void writeMBC5(const u16 addr, const u8 &data);
	void writeMBC6(const u16 addr, const u8 &data);
	void writeMBC7(const u16 addr, const u8 &data);
	void writeHuC1(const u16 addr, const u8 &data);

	void create_sav_file();
	void load_sav_file();
	void build_save_file_path();

	u8 read_card_rom(const u16 &addr);
	[[nodiscard]] unsigned int get_selected_rom_bank() const { return mbc1_selected_rom_bank; }

	std::vector<u8> rom;
	std::vector<u8> boot;
	std::vector<std::vector<u8>> ram;

	u32 mbc1_Rom_Ram_Mode;
	u32 mbc1_selected_ram_bank = 0;
	u32 mbc1_selected_rom_bank = 0;

	u32 mbc5_selected_ram_bank = 0;
	u32 mbc5_selected_rom_bank = 0;
	u32 mbc5_selected_rom_bank_hi = 0;

private:
	void fill_cartridge_info();
	void offset_bank_if_special(u32 &data);
	bool load_boot_rom();
	bool ram_enabled;
};

#endif
