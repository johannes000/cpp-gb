#pragma once
#ifndef GAMEBOY_H_GUARD
#define GAMEBOY_H_GUARD


#include "PPU.h"
#include "CPU.h"

class gameboy {
public:
	PPU ppu;
	CPU cpu;
	gameboy();
	void startemu();
private:
};

#endif // !GAMEBOY_H_GUARD
