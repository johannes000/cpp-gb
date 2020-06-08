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
	u8 checkSum = 0;
	u8 calcCheckSum();

public:
	char title[15] = "";
	bool SGBFlag = false;
	CGBFlag cgb = CGBFlag::ON;
	CType ctype = CType::MBC1;
	ROMSize romsize = ROMSize::S128KByte;
	RAMSize ramsize = RAMSize::None;

};


#endif