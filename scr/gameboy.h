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
	int run_one_cycle(bool skip_render);
	int run_one_cycle();
	void run_until_breakpoint();

	void reset(bool);

	bool load_rom(const char *, bool);
	void log_cpu(CPU_Register);
	void run_for_x_cycles(int);

	bool in_test_mode = false;
};

#endif // !GAMEBOY_H_GUARD
