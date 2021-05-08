#ifndef CARDRIDGE_H_GUARD
#define CARDRIDGE_H_GUARD

#include <vector>

#include "type.h"

constexpr u16 CARD_ROM_BANK_SIZE = 0x4000;
constexpr u16 CARD_RAM_BANK_SIZE = 0x4000;

class CARDRIDGE {
public:
	CARDRIDGE();
	bool rom_loaded = false;
	bool load_rom(const char *rom_file, bool skip_boot);

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
	void fill_cardridge_info();
	void offset_bank_if_special(u32 &data);
	bool load_boot_rom();
	bool ram_enabled;
};

#endif
