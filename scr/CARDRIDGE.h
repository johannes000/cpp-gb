#pragma once
#ifndef CARDRIDGE_H_GUARD
#define CARDRIDGE_H_GUARD

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "type.h"
#include "MEMORY.h"

class MEMORY;

class CARDRIDGE {
public:
	CARDRIDGE(MEMORY*);
	~CARDRIDGE();

	MEMORY* mem = nullptr;

	bool loadROM(const char*);
	void getRomInfo();
	void printRom();
	u8 readAddr(u16);

private:
	std::vector<u8> rom;
	void romintomem();
	void romintomem(u16 romBeg, u16 romEnd, u16 dest);
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
	}rom_info;
};


#endif