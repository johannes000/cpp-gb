#pragma once
#ifndef GAMEBOY_H_GUARD
#define GAMEBOY_H_GUARD


#include "PPU.h"
#include "CPU.h"
#include "MEMORY.h"
#include "CARDRIDGE.h"

class gameboy {
public:
	gameboy();
	~gameboy();

	PPU ppu;
	CARDRIDGE rom;
	MEMORY mem = (&rom);
	CPU cpu = (&mem);


	void startemu();
private:
};

#endif // !GAMEBOY_H_GUARD
