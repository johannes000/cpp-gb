#include "CARDRIDGE.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include "spdlog/spdlog.h"

CARDRIDGE::CARDRIDGE() {}

bool CARDRIDGE::load_rom(std::string rom_file, bool skip_boot) {
	rom.clear();
	ram.clear();
	boot.clear();

	if (!skip_boot)
		load_boot_rom();

	std::ifstream is(rom_file, std::ios::binary);
	if (!is) {
		spdlog::error("Kann die Datei nicht öffnen: {}", rom_file);
		return false;
	}
	const std::vector<u8> romf(std::istreambuf_iterator<char>(is), {});
	this->rom = romf;

	size_t last_slash_idx = rom_file.find_last_of("/");
	if (std::string::npos != last_slash_idx) {
		rom_info.filename = rom_file.substr(last_slash_idx + 1);
		rom_info.filepath = rom_file.substr(0, last_slash_idx);
	} else {
		last_slash_idx = rom_file.find_last_of("\\");
		if (std::string::npos != last_slash_idx) {
			rom_info.filename = rom_file.substr(last_slash_idx + 1);
			rom_info.filepath = rom_file.substr(0, last_slash_idx);
		} else
			rom_info.filename = rom_file;
	}
	build_save_file_path();
	fill_cardridge_info();

	ram = {static_cast<u16>(rom_info.ram_banks), {std::vector<u8>(CARD_RAM_BANK_SIZE, 0xFF)}};

	mbc1_Rom_Ram_Mode = 0;
	mbc1_selected_ram_bank = 0;
	mbc1_selected_rom_bank = 0;

	mbc5_selected_ram_bank = 0;
	mbc5_selected_rom_bank = 1;
	mbc5_selected_rom_bank_hi = 0;

	ram_enabled = false;
	rom_loaded = true;
	return true;
}

u8 CARDRIDGE::readBoot(const u16 &addr) {
	return boot.at(addr);
}

void CARDRIDGE::fill_cardridge_info() {
	if (rom.empty())
		return;

	memcpy(rom_info.title, &rom[0x134], 15);
	rom_info.cgb = static_cast<CGBFlag>(rom[0x143]);
	rom_info.ctype = static_cast<CType>(rom[0x147]);

	rom_info.rom_size = static_cast<ROMSize>(rom[0x148]);
	switch (rom_info.rom_size) {
		case ROMSize::KB32:
			rom_info.rom_banks = 2;
			break;
		case ROMSize::KB64:
			rom_info.rom_banks = 4;
			break;
		case ROMSize::KB128:
			rom_info.rom_banks = 8;
			break;
		case ROMSize::KB256:
			rom_info.rom_banks = 16;
			break;
		case ROMSize::KB512:
			rom_info.rom_banks = 32;
			break;
		case ROMSize::MB1:
			rom_info.rom_banks = 64;
			break;
		case ROMSize::MB2:
			rom_info.rom_banks = 128;
			break;
		case ROMSize::MB4:
			rom_info.rom_banks = 256;
			break;
		case ROMSize::MB8:
			rom_info.rom_banks = 512;
			break;
		case ROMSize::MB1_1:
			rom_info.rom_banks = 72;
			break;
		case ROMSize::MB1_2:
			rom_info.rom_banks = 80;
			break;
		case ROMSize::MB1_5:
			rom_info.rom_banks = 96;
			break;
		default:
			rom_info.rom_banks = 512 * 4;
			break;
	}

	rom_info.ram_size = static_cast<RAMSize>(rom[0x0149]);
	switch (rom_info.ram_size) {
		case RAMSize::None:
		case RAMSize::KB2:
		case RAMSize::KB8:
			rom_info.ram_banks = 1;
			break;
		case RAMSize::KB32:
			rom_info.ram_banks = 4;
			break;
		case RAMSize::KB128:
			rom_info.ram_banks = 16;
			break;
		case RAMSize::KB64:
			rom_info.ram_banks = 8;
			break;
		default:
			break;
	}
}

u8 CARDRIDGE::read(const u16 &addr) {

	switch (rom_info.ctype) {
		case CType::ROM_RAM:
		case CType::ROM_RAM_BATTERY:
		case CType::MBC1:
		case CType::MBC1_RAM:
		case CType::MBC1_RAM_BATTERY:
			return readMBC1(addr);
			break;
		case CType::MBC2:
		case CType::MBC2_BATTERY:
			return readMBC2(addr);
			break;
		case CType::MBC3_TIMER_BATTERY:
		case CType::MBC3_TIMER_RAM_BATTERY:
		case CType::MBC3:
		case CType::MBC3_RAM:
		case CType::MBC3_RAM_BATTERY:
			return readMBC3(addr);
			break;
		case CType::MBC5:
		case CType::MBC5_RAM:
		case CType::MBC5_RAM_BATTERY:
		case CType::MBC5_RUMBLE:
		case CType::MBC5_RUMBLE_RAM:
		case CType::MBC5_RUMBLE_RAM_BATTERY:
			return readMBC5(addr);
			break;
		case CType::MBC6:
			return readMBC6(addr);
			break;
		case CType::MBC7_SENSOR_RUMBLE_RAM_BATTERY:
			return readMBC7(addr);
			break;
		case CType::POCKET_CAMERA:
		case CType::BANDAI_TAMA5:
		case CType::HuC3:
		case CType::HUC1_RAM_BATTERY:
		case CType::MMM01:
		case CType::MMM01_RAM:
		case CType::MMM01_RAM_BATTERY:
		case CType::ROM_ONLY:
		default:
			return readNoMBC(addr);
			break;
	}
}

void CARDRIDGE::write(const u16 addr, const u8 &data) {
	if (addr <= 0x1FFF) {
		if ((data & 0xF) == 0xA)
			ram_enabled = true;
		else
			ram_enabled = false;
	}

	switch (rom_info.ctype) {
		case CType::MBC1:
		case CType::MBC1_RAM:
		case CType::MBC1_RAM_BATTERY:
			writeMBC1(addr, data);
			break;

		case CType::MBC2:
		case CType::MBC2_BATTERY:
			writeMBC2(addr, data);
			break;
		case CType::MBC3_TIMER_BATTERY:
		case CType::MBC3_TIMER_RAM_BATTERY:
		case CType::MBC3:
		case CType::MBC3_RAM:
		case CType::MBC3_RAM_BATTERY:
			writeMBC3(addr, data);
			break;
		case CType::MBC5:
		case CType::MBC5_RAM:
		case CType::MBC5_RAM_BATTERY:
		case CType::MBC5_RUMBLE:
		case CType::MBC5_RUMBLE_RAM:
		case CType::MBC5_RUMBLE_RAM_BATTERY:
			writeMBC5(addr, data);
			break;
		case CType::MBC6:
			writeMBC6(addr, data);
			break;
		case CType::MBC7_SENSOR_RUMBLE_RAM_BATTERY:
			writeMBC7(addr, data);
			break;
		case CType::POCKET_CAMERA:
		case CType::BANDAI_TAMA5:
		case CType::HuC3:
		case CType::HUC1_RAM_BATTERY:
		case CType::MMM01:
		case CType::MMM01_RAM:
		case CType::MMM01_RAM_BATTERY:
		case CType::ROM_RAM:
		case CType::ROM_RAM_BATTERY:
		case CType::ROM_ONLY:
		default:
			writeNoMBC(addr, data);
			break;
	}
}

u8 CARDRIDGE::read_card_rom(const u16 &addr) {
	if (addr < rom.size())
		return rom.at(addr);
	else
		return read(addr);
}

u8 CARDRIDGE::readNoMBC(const u16 &addr) {
	if (addr <= CARD_BANKS_END) {
		return rom.at(addr);
	} else {
		return 0xFF;
	}
}

u8 CARDRIDGE::readMBC2(const u16 &addr) {
	// TODO: Implementieren
	return readNoMBC(addr);
}
u8 CARDRIDGE::readMBC3(const u16 &addr) {
	// TODO: Implementieren
	return readNoMBC(addr);
}

u8 CARDRIDGE::readMBC6(const u16 &addr) {
	// TODO: Implementieren
	return readNoMBC(addr);
}
u8 CARDRIDGE::readMBC7(const u16 &addr) {
	// TODO: Implementieren
	return readNoMBC(addr);
}
u8 CARDRIDGE::readHuC1(const u16 &addr) {
	// TODO: Implementieren
	return readNoMBC(addr);
}
void CARDRIDGE::writeNoMBC(const u16 addr, const u8 &data) {
	if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END && rom_info.ram_size != RAMSize::None && ram_enabled) {
		ram[0].at(addr - CARD_EXTERNAL_RAM) = data;
	} else {
		//std::cerr << "Hier Sollte nicht Reingeschrieben werden! Addr:" << addr << std::endl;
	}
}

u8 CARDRIDGE::readMBC1(const u16 &addr) {
	if (rom_info.rom_banks == 0) {
		return readNoMBC(addr);
	}

	if (addr <= CARD_ROM_0_END) {
		u32 addr_in_rom = addr;
		if (mbc1_Rom_Ram_Mode) {
			u32 real_bank = (((mbc1_selected_ram_bank & 3) << 5) % rom_info.rom_banks);
			offset_bank_if_special(real_bank);
			addr_in_rom = addr + (CARD_ROM_BANK_SIZE * (real_bank - 1));
		}
		return rom.at(addr_in_rom % rom.size());

	} else if (addr >= CARD_BANKS && addr <= CARD_BANKS_END) {
		/*
		4000 - 7FFF - ROM Bank 01 - 7F(Read Only)
		This area may contain any of the further 16KByte banks of the ROM,
		allowing to address up to 125 ROM Banks(almost 2MByte).
		As described below, bank numbers 20h, 40h, and 60h cannot be used, resulting in the odd amount of 125 banks.
		*/
		u32 real_bank = (((mbc1_selected_ram_bank & 3) << 5) % rom_info.rom_banks) | mbc1_selected_rom_bank;
		offset_bank_if_special(real_bank);

		u32 addr_in_rom = addr + (CARD_ROM_BANK_SIZE * (real_bank - 1));

		return rom.at(addr_in_rom % rom.size());
	} else if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END) {
		/*
		A000-BFFF - RAM Bank 00-03, if any (Read/Write)
		This area is used to address external RAM in the cartridge (if any).
		External RAM is often battery buffered, allowing to store game positions or high score tables,
		even if the gameboy is turned off, or if the cartridge is removed from the gameboy.
		Available RAM sizes are: 2KByte (at A000-A7FF), 8KByte (at A000-BFFF), and 32KByte (in form of four 8K banks at A000-BFFF).
		*/
		if (ram_enabled) {
			s32 real_bank = mbc1_selected_ram_bank;
			if (!mbc1_Rom_Ram_Mode) {
				real_bank = 0;
			}
			return ram[real_bank].at(addr - CARD_EXTERNAL_RAM);
		} else {
			return 0xFF;
		}
	}

	return readNoMBC(addr);
}

void CARDRIDGE::writeMBC1(const u16 addr, const u8 &data) {
	if (addr >= 0x2000 && addr <= 0x3FFF && rom_info.rom_banks > 0) {
		// 2000 - 3FFF - ROM Bank Number(Write Only)
		int rom_bank = (data & 0x1F);
		mbc1_selected_rom_bank = rom_bank;
	}

	if (addr >= 0x4000 && addr <= 0x5FFF) {
		// 4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only)
		mbc1_selected_ram_bank = data & 3;
		if (mbc1_Rom_Ram_Mode == 1 && static_cast<int>(ram.size()) < mbc1_selected_ram_bank) {
			// Spiel versucht auf Ram banks zuzugreifen die eigentlich gar nicht da sind.
			ram.insert(ram.end(), mbc1_selected_ram_bank - ram.size() + 1, {std::vector<u8>(CARD_RAM_BANK_SIZE, 0xFF)});
		}
	}
	if (addr >= 0x6000 && addr <= 0x7FFF) {
		// 6000-7FFF - ROM/RAM Mode Select (Write Only)
		switch (data) {
			case 0x00:
				mbc1_Rom_Ram_Mode = 0;
				break;
			case 0x01:
				mbc1_Rom_Ram_Mode = 1;
				break;
			default:
				break;
		}
	}

	if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END && ram_enabled) {
		if (mbc1_Rom_Ram_Mode == 1)
			ram[mbc1_selected_ram_bank].at(addr - CARD_EXTERNAL_RAM) = data;
		else
			ram[0].at(addr - CARD_EXTERNAL_RAM) = data;
	}
}
void CARDRIDGE::writeMBC2(const u16 addr, const u8 &data) {
	writeNoMBC(addr, data);
	// TODO: Implementieren
}
void CARDRIDGE::writeMBC3(const u16 addr, const u8 &data) {
	writeNoMBC(addr, data);
	// TODO: Implementieren
}
u8 CARDRIDGE::readMBC5(const u16 &addr) {
	if (addr <= CARD_ROM_0_END) {
		return rom.at(addr);
	} else if (addr >= 0x4000 && addr <= 0x7FFF) {
		u32 addr_in_rom = (addr - 0x4000) + (CARD_ROM_BANK_SIZE * mbc5_selected_rom_bank);
		return rom[addr_in_rom];
	} else if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END && ram_enabled) {
		return ram[mbc5_selected_ram_bank].at(addr - CARD_EXTERNAL_RAM);
	}
	return 0xFF;
}
void CARDRIDGE::writeMBC5(const u16 addr, const u8 &data) {
	if (addr >= 0x2000 && addr <= 0x2FFF) {
		mbc5_selected_rom_bank = data | (mbc5_selected_rom_bank_hi << 8);
		mbc5_selected_rom_bank &= (rom_info.rom_banks - 1);

	} else if (addr >= 0x3000 && addr <= 0x3FFF) {
		mbc5_selected_rom_bank_hi = data & 0x01;
		mbc5_selected_rom_bank = (mbc5_selected_rom_bank & 0xFF) | (mbc5_selected_rom_bank_hi << 8);
		mbc5_selected_rom_bank &= (rom_info.rom_banks - 1);
	} else if (addr >= 0x4000 && addr <= 0x5FFF) {
		mbc5_selected_ram_bank = data & 0x0F;
	} else if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END && ram_enabled) {
		ram[mbc5_selected_ram_bank % 0x0F].at(addr - CARD_EXTERNAL_RAM) = data;
	}
	// TODO: Implementieren
}
void CARDRIDGE::writeMBC6(const u16 addr, const u8 &data) {
	writeNoMBC(addr, data);
	// TODO: Implementieren
}
void CARDRIDGE::writeMBC7(const u16 addr, const u8 &data) {
	writeNoMBC(addr, data);
	// TODO: Implementieren
}
void CARDRIDGE::writeHuC1(const u16 addr, const u8 &data) {
	writeNoMBC(addr, data);
	// TODO: Implementieren
}
void CARDRIDGE::offset_bank_if_special(u32 &data) {
	if (data == 0x0 || data == 0x20 || data == 0x40 || data == 0x60 || data == 0x80)
		data++;
}

bool CARDRIDGE::load_boot_rom() {
	boot.clear();

	const char *boot_rom_path = "C:/Users/knaub/source/repos/testroms/DMG_ROM.bin";
	//const char *boot_rom_path = "C:/Users/knaub/source/repos/testroms/gbc_bios.bin";
	std::ifstream is(boot_rom_path, std::ios::binary);
	if (!is) {
		std::cerr << "Kann die Boot Rom nicht öffnen!" << std::endl;
		std::cerr << boot_rom_path << std::endl;
		abort();
	} else {
		const std::vector<u8> romf(std::istreambuf_iterator<char>(is), {});
		boot = romf;
		return true;
	}
	return false;
}

void CARDRIDGE::build_save_file_path() {

	std::string file;
	std::string path;

	path = rom_info.filepath;
	path += "/";

	auto it = rom_info.filename.find(".");
	if (it != std::string::npos)
		file = rom_info.filename.substr(0, it);
	path += file;
	path += ".sav";
	rom_info.savefile = path;
}

void CARDRIDGE::create_sav_file() {
	if (rom_info.ram_banks == 0 || !rom_loaded)
		return;

	std::string file = rom_info.savefile;
	spdlog::info("Saving Ram");
	spdlog::info("Save File: {}", file);

	std::ofstream os(file.c_str(), std::ios::out | std::ios::binary);

	for (s32 i = 0; i < rom_info.ram_banks; i++) {
		for (auto data : ram[i]) {
			os.write(reinterpret_cast<const char *>(&data), 1);
		}
	}
	spdlog::info("Ram Save done");
}

void CARDRIDGE::load_sav_file() {
	if (rom_info.ram_banks == 0)
		return;
	std::string file = rom_info.savefile;

	spdlog::info("Loading Ram");
	spdlog::info("Save File: {}", file);
	std::ifstream is(file, std::ios::in | std::ios::binary);
	for (s32 i = 0; i < rom_info.ram_banks; i++) {
		int index = 0;
		while (index < CARD_RAM_BANK_SIZE) {
			u8 byte = 0;
			is.read(reinterpret_cast<char *>(&byte), 1);
			ram[i][index++] = byte;
		}
	}
	spdlog::info("Ram Load done");
}