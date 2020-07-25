#ifndef GAMEBOY_H_GUARD
#define GAMEBOY_H_GUARD

#include "CARDRIDGE.h"
#include "CPU.h"
#include "MEMORY.h"
#include "PPU.h"
#include "CONTROLS.h"
#include "Debugger/Debugger.h"

struct gameboy {
	gameboy();

	CARDRIDGE card;
	MEMORY mem;
	CPU cpu;
	PPU ppu;
	CONTROLS joypad;
	Debugger debug;

	void run_until_vblank();
	void run_one_cycle();

	bool load_rom(const char*);
};

#endif  // !GAMEBOY_H_GUARD
