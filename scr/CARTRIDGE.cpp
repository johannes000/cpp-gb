#include "CARTRIDGE.hpp"

#include <fstream>
#include <iostream>
#include <cstring>

CARTRIDGE::CARTRIDGE() {}

auto CARTRIDGE::load_rom(std::string rom_file, bool skip_boot) -> bool {
	rom_.clear();
	ram_.clear();
	boot_.clear();

	if (!skip_boot)
		load_boot_rom();

	std::ifstream is(rom_file, std::ios::binary);
	if (!is) {
		spdlog::error("Kann die Datei nicht öffnen: {}", rom_file.c_str());
		return false;
	}
	const std::vector<u8> romf(std::istreambuf_iterator<char>(is), {});
	this->rom_ = romf;

	size_t last_slash_idx = rom_file.find_last_of("/");
	if (std::string::npos != last_slash_idx) {
		rom_info_.filename_ = rom_file.substr(last_slash_idx + 1);
		rom_info_.filepath_ = rom_file.substr(0, last_slash_idx);
	} else {
		last_slash_idx = rom_file.find_last_of("\\");
		if (std::string::npos != last_slash_idx) {
			rom_info_.filename_ = rom_file.substr(last_slash_idx + 1);
			rom_info_.filepath_ = rom_file.substr(0, last_slash_idx);
		} else
			rom_info_.filename_ = rom_file;
	}
	build_save_file_path();
	fill_cartridge_info();

	ram_ = {static_cast<u16>(rom_info_.ram_banks_), {std::vector<u8>(CARD_RAM_BANK_SIZE, 0xFF)}};

	mbc1_Rom_Ram_Mode_ = 0;
	mbc1_selected_ram_bank_ = 0;
	mbc1_selected_rom_bank_ = 0;

	mbc5_selected_ram_bank_ = 0;
	mbc5_selected_rom_bank_ = 1;
	mbc5_selected_rom_bank_hi_ = 0;

	ram_enabled_ = false;
	rom_loaded_ = true;
	return true;
}

auto CARTRIDGE::readBoot(const u16 &addr) -> u8 {
	return boot_.at(addr);
}

auto CARTRIDGE::fill_cartridge_info() -> void {
	if (rom_.empty())
		return;

	memcpy(rom_info_.title_, &rom_[0x134], 15);
	rom_info_.cgb_ = static_cast<CGBFlag>(rom_[0x143]);
	rom_info_.ctype_ = static_cast<CType>(rom_[0x147]);

	rom_info_.rom_size_ = static_cast<ROMSize>(rom_[0x148]);
	switch (rom_info_.rom_size_) {
		case ROMSize::KB32:
			rom_info_.rom_banks_ = 2;
			break;
		case ROMSize::KB64:
			rom_info_.rom_banks_ = 4;
			break;
		case ROMSize::KB128:
			rom_info_.rom_banks_ = 8;
			break;
		case ROMSize::KB256:
			rom_info_.rom_banks_ = 16;
			break;
		case ROMSize::KB512:
			rom_info_.rom_banks_ = 32;
			break;
		case ROMSize::MB1:
			rom_info_.rom_banks_ = 64;
			break;
		case ROMSize::MB2:
			rom_info_.rom_banks_ = 128;
			break;
		case ROMSize::MB4:
			rom_info_.rom_banks_ = 256;
			break;
		case ROMSize::MB8:
			rom_info_.rom_banks_ = 512;
			break;
		case ROMSize::MB1_1:
			rom_info_.rom_banks_ = 72;
			break;
		case ROMSize::MB1_2:
			rom_info_.rom_banks_ = 80;
			break;
		case ROMSize::MB1_5:
			rom_info_.rom_banks_ = 96;
			break;
		default:
			rom_info_.rom_banks_ = 512 * 4;
			break;
	}

	rom_info_.ram_size_ = static_cast<RAMSize>(rom_[0x0149]);
	switch (rom_info_.ram_size_) {
		case RAMSize::None:
		case RAMSize::KB2:
		case RAMSize::KB8:
			rom_info_.ram_banks_ = 1;
			break;
		case RAMSize::KB32:
			rom_info_.ram_banks_ = 4;
			break;
		case RAMSize::KB128:
			rom_info_.ram_banks_ = 16;
			break;
		case RAMSize::KB64:
			rom_info_.ram_banks_ = 8;
			break;
		default:
			break;
	}
}

auto CARTRIDGE::read(const u16 &addr) -> u8 {

	switch (rom_info_.ctype_) {
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

auto CARTRIDGE::write(const u16 addr, const u8 &data) -> void {
	if (addr <= 0x1FFF) {
		if ((data & 0xF) == 0xA)
			ram_enabled_ = true;
		else
			ram_enabled_ = false;
	}

	switch (rom_info_.ctype_) {
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

auto CARTRIDGE::read_card_rom(const u16 &addr) -> u8 {
	if (addr < rom_.size())
		return rom_.at(addr);
	else
		return read(addr);
}

auto CARTRIDGE::readNoMBC(const u16 &addr) -> u8 {
	if (addr <= CARD_BANKS_END) {
		return rom_.at(addr);
	} else {
		return 0xFF;
	}
}

auto CARTRIDGE::readMBC2(const u16 &addr) -> u8 {
	// TODO: Implementieren
	return readNoMBC(addr);
}
auto CARTRIDGE::readMBC3(const u16 &addr) -> u8 {
	// TODO: Implementieren
	return readNoMBC(addr);
}

auto CARTRIDGE::readMBC6(const u16 &addr) -> u8 {
	// TODO: Implementieren
	return readNoMBC(addr);
}
auto CARTRIDGE::readMBC7(const u16 &addr) -> u8 {
	// TODO: Implementieren
	return readNoMBC(addr);
}
auto CARTRIDGE::readHuC1(const u16 &addr) -> u8 {
	// TODO: Implementieren
	return readNoMBC(addr);
}
auto CARTRIDGE::writeNoMBC(const u16 addr, const u8 &data) -> void {
	if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END && rom_info_.ram_size_ != RAMSize::None && ram_enabled_) {
		ram_[0].at(addr - CARD_EXTERNAL_RAM) = data;
	} else {
		// std::cerr << "Hier Sollte nicht Reingeschrieben werden! Addr:" << addr << std::endl;
	}
}

auto CARTRIDGE::readMBC1(const u16 &addr) -> u8 {
	if (rom_info_.rom_banks_ == 0) {
		return readNoMBC(addr);
	}

	if (addr <= CARD_ROM_0_END) {
		u32 addr_in_rom = addr;
		if (mbc1_Rom_Ram_Mode_) {
			u32 real_bank = (((mbc1_selected_ram_bank_ & 3) << 5) % rom_info_.rom_banks_);
			offset_bank_if_special(real_bank);
			addr_in_rom = addr + (CARD_ROM_BANK_SIZE * (real_bank - 1));
		}
		return rom_.at(addr_in_rom % rom_.size());

	} else if (addr <= CARD_BANKS_END) {
		/*
		4000 - 7FFF - ROM Bank 01 - 7F(Read Only)
		This area may contain any of the further 16KByte banks of the ROM,
		allowing to address up to 125 ROM Banks(almost 2MByte).
		As described below, bank numbers 20h, 40h, and 60h cannot be used, resulting in the odd amount of 125 banks.
		*/
		u32 real_bank = (((mbc1_selected_ram_bank_ & 3) << 5) % rom_info_.rom_banks_) | mbc1_selected_rom_bank_;
		offset_bank_if_special(real_bank);

		u32 addr_in_rom = addr + (CARD_ROM_BANK_SIZE * (real_bank - 1));

		return rom_.at(addr_in_rom % rom_.size());
	} else if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END) {
		/*
		A000-BFFF - RAM Bank 00-03, if any (Read/Write)
		This area is used to address external RAM in the cartridge (if any).
		External RAM is often battery buffered, allowing to store game positions or high score tables,
		even if the gameboy is turned off, or if the cartridge is removed from the gameboy.
		Available RAM sizes are: 2KByte (at A000-A7FF), 8KByte (at A000-BFFF), and 32KByte (in form of four 8K banks at A000-BFFF).
		*/
		if (ram_enabled_) {
			s32 real_bank = mbc1_selected_ram_bank_;
			if (!mbc1_Rom_Ram_Mode_) {
				real_bank = 0;
			}
			return ram_[real_bank].at(addr - CARD_EXTERNAL_RAM);
		} else {
			return 0xFF;
		}
	}

	return readNoMBC(addr);
}

auto CARTRIDGE::writeMBC1(const u16 addr, const u8 &data) -> void {
	if (addr >= 0x2000 && addr <= 0x3FFF && rom_info_.rom_banks_ > 0) {
		// 2000 - 3FFF - ROM Bank Number(Write Only)
		int rom_bank = (data & 0x1F);
		mbc1_selected_rom_bank_ = rom_bank;
	}

	if (addr >= 0x4000 && addr <= 0x5FFF) {
		// 4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only)
		mbc1_selected_ram_bank_ = data & 3;
		if (mbc1_Rom_Ram_Mode_ == 1 && static_cast<u32>(ram_.size()) < mbc1_selected_ram_bank_) {
			// Spiel versucht auf Ram banks zuzugreifen die eigentlich gar nicht da sind.
			ram_.insert(ram_.end(), mbc1_selected_ram_bank_ - ram_.size() + 1, {std::vector<u8>(CARD_RAM_BANK_SIZE, 0xFF)});
		}
	}
	if (addr >= 0x6000 && addr <= 0x7FFF) {
		// 6000-7FFF - ROM/RAM Mode Select (Write Only)
		switch (data) {
			case 0x00:
				mbc1_Rom_Ram_Mode_ = 0;
				break;
			case 0x01:
				mbc1_Rom_Ram_Mode_ = 1;
				break;
			default:
				break;
		}
	}

	if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END && ram_enabled_) {
		if (mbc1_Rom_Ram_Mode_ == 1)
			ram_[mbc1_selected_ram_bank_].at(addr - CARD_EXTERNAL_RAM) = data;
		else
			ram_[0].at(addr - CARD_EXTERNAL_RAM) = data;
	}
}
auto CARTRIDGE::writeMBC2(const u16 addr, const u8 &data) -> void {
	writeNoMBC(addr, data);
	// TODO: Implementieren
}
auto CARTRIDGE::writeMBC3(const u16 addr, const u8 &data) -> void {
	writeNoMBC(addr, data);
	// TODO: Implementieren
}
auto CARTRIDGE::readMBC5(const u16 &addr) -> u8 {
	if (addr <= CARD_ROM_0_END) {
		return rom_.at(addr);
	} else if (addr <= 0x7FFF) {
		u32 addr_in_rom = (addr - 0x4000) + (CARD_ROM_BANK_SIZE * mbc5_selected_rom_bank_);
		return rom_[addr_in_rom];
	} else if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END && ram_enabled_) {
		return ram_[mbc5_selected_ram_bank_].at(addr - CARD_EXTERNAL_RAM);
	}
	return 0xFF;
}
auto CARTRIDGE::writeMBC5(const u16 addr, const u8 &data) -> void {
	if (addr >= 0x2000 && addr <= 0x2FFF) {
		mbc5_selected_rom_bank_ = data | (mbc5_selected_rom_bank_hi_ << 8);
		mbc5_selected_rom_bank_ &= (rom_info_.rom_banks_ - 1);

	} else if (addr >= 0x3000 && addr <= 0x3FFF) {
		mbc5_selected_rom_bank_hi_ = data & 0x01;
		mbc5_selected_rom_bank_ = (mbc5_selected_rom_bank_ & 0xFF) | (mbc5_selected_rom_bank_hi_ << 8);
		mbc5_selected_rom_bank_ &= (rom_info_.rom_banks_ - 1);
	} else if (addr >= 0x4000 && addr <= 0x5FFF) {
		mbc5_selected_ram_bank_ = data & 0x0F;
	} else if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END && ram_enabled_) {
		ram_[mbc5_selected_ram_bank_ % 0x0F].at(addr - CARD_EXTERNAL_RAM) = data;
	}
	// TODO: Implementieren
}
auto CARTRIDGE::writeMBC6(const u16 addr, const u8 &data) -> void {
	writeNoMBC(addr, data);
	// TODO: Implementieren
}
auto CARTRIDGE::writeMBC7(const u16 addr, const u8 &data) -> void {
	writeNoMBC(addr, data);
	// TODO: Implementieren
}
auto CARTRIDGE::writeHuC1(const u16 addr, const u8 &data) -> void {
	writeNoMBC(addr, data);
	// TODO: Implementieren
}
auto CARTRIDGE::offset_bank_if_special(u32 &data) -> void {
	if (data == 0x0 || data == 0x20 || data == 0x40 || data == 0x60 || data == 0x80)
		data++;
}

auto CARTRIDGE::load_boot_rom() -> bool {
	boot_.clear();

	const char *boot_rom_path = "C:/Users/knaub/source/repos/testroms/DMG_ROM.bin";
	// const char *boot_rom_path = "C:/Users/knaub/source/repos/testroms/gbc_bios.bin";
	std::ifstream is(boot_rom_path, std::ios::binary);
	if (!is) {
		spdlog::error("Kann die Boot Rom nicht öffnen! {}", boot_rom_path);
		abort();
	} else {
		const std::vector<u8> romf(std::istreambuf_iterator<char>(is), {});
		boot_ = romf;
		return true;
	}
	return false;
}

auto CARTRIDGE::build_save_file_path() -> void {

	std::string file;
	std::string path;

	path = rom_info_.filepath_;
	path += "/";

	auto it = rom_info_.filename_.find(".");
	if (it != std::string::npos)
		file = rom_info_.filename_.substr(0, it);
	path += file;
	path += ".sav";
	rom_info_.savefile_ = path;
}

auto CARTRIDGE::create_sav_file() -> void {
	if (rom_info_.ram_banks_ == 0 || !rom_loaded_)
		return;

	std::string file = rom_info_.savefile_;

	std::ofstream os(file.c_str(), std::ios::out | std::ios::binary);

	for (s32 i = 0; i < rom_info_.ram_banks_; i++) {
		for (auto data : ram_[i]) {
			os.write(reinterpret_cast<const char *>(&data), 1);
		}
	}
	spdlog::info("Sav File created: {}", file.c_str());
}

auto CARTRIDGE::load_sav_file() -> void {
	if (rom_info_.ram_banks_ == 0)
		return;
	std::string file = rom_info_.savefile_;

	std::ifstream is(file, std::ios::in | std::ios::binary);
	for (s32 i = 0; i < rom_info_.ram_banks_; i++) {
		int index = 0;
		while (index < CARD_RAM_BANK_SIZE) {
			u8 byte = 0;
			is.read(reinterpret_cast<char *>(&byte), 1);
			ram_[i][index++] = byte;
		}
	}
	spdlog::info("Sav File Loaded: {}", file.c_str());
}