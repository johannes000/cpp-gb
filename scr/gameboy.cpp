#include "gameboy.h"

gameboy::gameboy()
	: mem(&cpu, &card, &ppu, &joypad),
	  cpu(&mem),
	  ppu(&mem, &cpu),
	  joypad(&mem),
	  debug(&cpu, &ppu, &mem, &card, &joypad) {
	reset(false);
}

bool gameboy::load_rom(const char *rom_file, bool skip_boot_rom) {
	reset(skip_boot_rom);

	if (!*rom_file)
		return false;
	card.load_rom(rom_file);

	return true;
}
void gameboy::reset(bool skip_boot_rom) {
	mem.reset(skip_boot_rom);
	cpu.init_register(skip_boot_rom);
	ppu.reset();
}

void gameboy::run_one_cycle() {
	run_one_cycle(false);
}

int gameboy::run_one_cycle(bool skip_render) {
#if 0
	auto aa = cpu.get_register();
	log_cpu(aa);
#endif
	const int last_cycle = cpu.update();
	cpu.handle_interrupts();
	cpu.handle_timer(last_cycle);
	ppu.update(last_cycle * 4, skip_render);
	return last_cycle;
}

void gameboy::log_cpu(CPU_Register R) {
	static int a = 0;

	if (!cpu.is_in_boot()) {
		printf("A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X", R.A, R.F, R.B, R.C, R.D, R.E, R.H, R.L);
		printf(" SP: %04X", R.SP);
		printf(" PC: 00:%04X", R.PC);
		printf(" (%02X %02X %02X %02X)", mem.read(R.PC), mem.read(R.PC + 1), mem.read(R.PC + 2), mem.read(R.PC + 3));
		printf("\n");
		a++;
	}
}

void gameboy::run_until_breakpoint() {
	while (!ppu.is_ready_to_render() &&
		   debug.get_debug_state() == Debug_State::BREAKPOINT) {
		run_one_cycle();
		debug.handle_breakpoints();
	}
}

void gameboy::run_for_x_cycles(int cycles) {
	int cycles_sum = 0;
	while (cycles_sum < cycles) {
		cycles_sum += run_one_cycle(true);
	}
}
void gameboy::run_until_vblank() {
	while (!ppu.is_ready_to_render() &&
		   debug.get_debug_state() == Debug_State::RUN) {
		run_one_cycle();
	}
}
