#ifndef GAMEBOY_H_GUARD
#define GAMEBOY_H_GUARD

#include <string>

#include "CARTRIDGE.h"
#include "MEMORY.h"
#include "CPU.h"
#include "PPU.h"
#include "CONTROLS.h"
#include "APU.h"

struct gameboy {
	gameboy();

	CARTRIDGE card;
	MEMORY mem;
	CPU cpu;
	PPU ppu;
	CONTROLS joypad;
	APU apu;

	void run_until_vblank();
	void run_until_soundbuffer_full(i32 Sample_Size);
	int run_one_cycle(bool skip_render);
	int run_one_cycle();

	void reset(bool);

	bool load_rom(std::string, bool);
	void log_cpu(CPU_Register);
	void run_for_x_cycles(int);

	bool in_test_mode;
	bool skip_boot_rom;
	bool fps_drossel;
};

#endif // !GAMEBOY_H_GUARD
