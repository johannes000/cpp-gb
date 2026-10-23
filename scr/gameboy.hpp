#ifndef GAMEBOY_H_GUARD
#define GAMEBOY_H_GUARD

#include <string>

#include "CARTRIDGE.hpp"
#include "MEMORY.hpp"
#include "CPU.hpp"
#include "PPU.hpp"
#include "CONTROLS.hpp"
#include "APU.hpp"

struct gameboy {
	gameboy();

	CARTRIDGE card_;
	MEMORY mem_;
	CPU cpu_;
	PPU ppu_;
	CONTROLS joypad_;
	APU apu_;

	auto run_until_vblank() -> void;
	auto run_until_soundbuffer_full(i32 Sample_Size) -> void;
	auto run_one_cycle(bool skip_render) -> int;
	auto run_one_cycle() -> int;

	auto reset(bool) -> void;

	auto load_rom(std::string, bool) -> bool;
	auto log_cpu(CPU_Register) -> void;
	auto run_for_x_cycles(int) -> void;

	bool in_test_mode_;
	bool skip_boot_rom_;
	bool fps_drossel_;
};

#endif // !GAMEBOY_H_GUARD
