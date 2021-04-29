#include "CARDRIDGE.h"

#include <fstream>
#include <iostream>
#include <cstring>

CARDRIDGE::CARDRIDGE() {}

bool CARDRIDGE::load_rom(const char *rom_file) {
	rom.clear();
	ram.clear();

	std::ifstream is(rom_file, std::ios::binary);
	if (!is) {
		std::cerr << "Kann die Datei nicht oeffnen : " << rom_file << std::endl;
		return false;
	}
	const std::vector<u8> romf(std::istreambuf_iterator<char>(is), {});
	this->rom = romf;

	fill_cardridge_info();
	for (int i = 0; i < rom_info.ram_banks + 1; ++i) {
		ram.push_back(std::vector<u8>(CARD_RAM_BANK_SIZE, 0xFF));
	}
	selected_ram_bank = 0;
	selected_rom_bank = 0;
	ram_enabled = false;
	return true;
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
			rom_info.rom_banks = 1;
			break;
		case ROMSize::KB64:
			rom_info.rom_banks = 4;
			break;
		case ROMSize::KB128:
			rom_info.rom_banks = 8;
			break;
		case ROMSize::KB256:
		case ROMSize::KB512:
			rom_info.rom_banks = 16;
			break;
		case ROMSize::MB1:
			switch (rom_info.ctype) {
				case CType::MBC1:
					rom_info.rom_banks = 63;
					break;
				default:
					rom_info.rom_banks = 64;
					break;
			}
			break;
		case ROMSize::MB2:
			switch (rom_info.ctype) {
				case CType::MBC1:
					rom_info.rom_banks = 125;
					break;
				default:
					rom_info.rom_banks = 128;
					break;
			}
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
			break;
	}

	rom_info.ram_size = static_cast<RAMSize>(rom[0x149]);
	switch (rom_info.ram_size) {
		case RAMSize::None:
		case RAMSize::KB2:
		case RAMSize::KB8:
			rom_info.ram_banks = 0;
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
		case CType::ROM_RAM:
		case CType::ROM_RAM_BATTERY:
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

	if (addr >= 0x2000 && addr <= 0x3FFF && rom_info.rom_banks > 0) {
		selected_rom_bank = (data & 0x1F);
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
u8 CARDRIDGE::readMBC1(const u16 &addr) {
	if (rom_info.rom_banks == 0) {
		return readNoMBC(addr);
	}

	if (addr <= CARD_ROM_0_END) {
		return rom.at(addr);
	} else if (addr >= CARD_BANKS && addr <= CARD_BANKS_END) {
		int32 real_bank = selected_rom_bank;
		if (check_if_bank_special(selected_rom_bank))
			real_bank++;
		u32 addr_in_rom = addr + (CARD_ROM_BANK_SIZE * (real_bank - 1));
		return rom.at(addr_in_rom);
	} else if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END && ram_enabled) {
		return ram[selected_ram_bank].at(addr - CARD_EXTERNAL_RAM);
	}

	return readNoMBC(addr);
}
u8 CARDRIDGE::readMBC2(const u16 &addr) {
	// TODO: Implementieren
	return readNoMBC(addr);
}
u8 CARDRIDGE::readMBC3(const u16 &addr) {
	// TODO: Implementieren
	return readNoMBC(addr);
}
u8 CARDRIDGE::readMBC5(const u16 &addr) {
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
		std::cerr << "Hier Sollte nicht Reingeschrieben werden! Addr:" << addr << std::endl;
	}
}
void CARDRIDGE::writeMBC1(const u16 addr, const u8 &data) {
	if (addr >= CARD_EXTERNAL_RAM && addr <= CARD_EXTERNAL_RAM_END && ram_enabled) {
		ram[selected_ram_bank].at(addr - CARD_EXTERNAL_RAM) = data;
	}
	// TODO: Implementieren
}
void CARDRIDGE::writeMBC2(const u16 addr, const u8 &data) {
	writeNoMBC(addr, data);
	// TODO: Implementieren
}
void CARDRIDGE::writeMBC3(const u16 addr, const u8 &data) {
	writeNoMBC(addr, data);
	// TODO: Implementieren
}
void CARDRIDGE::writeMBC5(const u16 addr, const u8 &data) {
	writeNoMBC(addr, data);
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

bool CARDRIDGE::check_if_bank_special(int32 bank) {
	return (bank == 0x0 || bank == 0x20 || bank == 0x40 || bank == 0x60);
}