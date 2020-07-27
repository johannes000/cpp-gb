#include "gameboy.h"

gameboy::gameboy() :
	mem(&cpu, &card, &ppu, &joypad),
	cpu(&mem), ppu(&mem, &cpu),
	joypad(&mem),
	debug(&cpu, &ppu, &mem, &card, &joypad) {
}



bool gameboy::load_rom(const char* rom_file) {
	mem.reset();
	cpu.init_register();
	ppu.reset();

	if (rom_file == "") return false;
	card.load_rom(rom_file);

	return true;
}

void gameboy::run_one_cycle() {
	const int last_cycle = cpu.update();
	cpu.handle_interrupts();
	cpu.handle_timer(last_cycle);
	ppu.update(last_cycle * 4);
}

void gameboy::run_until_breakpoint() {
	while (!ppu.is_ready_to_render() && debug.get_debug_state() == Debug_State::BREAKPOINT) {
		run_one_cycle();
		debug.handle_breakpoints();
	}
}

void gameboy::run_until_vblank() {
	while (!ppu.is_ready_to_render()) {
		run_one_cycle();
	}
}


