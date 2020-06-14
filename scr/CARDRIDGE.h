#pragma once
#ifndef CARDRIDGE_H_GUARD
#define CARDRIDGE_H_GUARD

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "type.h"

constexpr u16 ROM_BANK_SIZE = 0x2000;
constexpr u16 RAM_BANK_SIZE = 0x4000;

class CARDRIDGE {
public:
	CARDRIDGE();
	~CARDRIDGE();

	bool loadROM(const char* romFile);

	u8 read(const u16& addr);
	u8* read_ptr(const u16& addr);
	void write(const u16& addr, const u8& data);
private:
	void fill_cardridge_info();
	void printRom();
	void build_ram();

	std::vector<u8> rom;
	std::vector<std::vector<u8>> ram;

	int selected_ram_bank = 0;
	int selected_rom_bank = 0;

	u8 checkSum = 0;
	u8 calcCheckSum();

public:
	struct Rom_Info {
		char title[15] = "";
		bool SGBFlag = false;

		CGBFlag cgb = CGBFlag::ON;
		CType ctype = CType::MBC1;

		ROMSize romsize = ROMSize::KB32;
		RAMSize ramsize = RAMSize::None;

		int rom_banks;
		int ram_banks;
	}rom_info;
};


#endif
