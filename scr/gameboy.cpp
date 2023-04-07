#include "gameboy.h"

#include "Debugger/Debugger.h"
#include "utl/type.h"

gameboy::gameboy()
	: mem(&cpu, &card, &ppu, &joypad, &apu),
	  cpu(&mem),
	  ppu(&mem, &cpu),
	  joypad(&mem),
	  apu(&mem),
	  debug(&cpu, &ppu, &mem, &card, &joypad, &apu),

	  in_test_mode(false),
	  skip_boot_rom(false),
	  fps_drossel(true) {
	reset(skip_boot_rom);
}

bool gameboy::load_rom(std::string rom_file, bool skip_boot) {
	reset(skip_boot);

	if (rom_file.empty())
		return false;
	card.load_rom(rom_file, skip_boot);

	return true;
}

void gameboy::reset(bool skip_boot) {
	mem.reset(skip_boot);
	cpu.init_register(skip_boot);
	ppu.reset();
}

int gameboy::run_one_cycle() {
	return run_one_cycle(false);
}

void gameboy::log_cpu(CPU_Register R) {
	if (!cpu.is_in_boot()) {
		printf("A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X", R.A, R.F, R.B, R.C, R.D, R.E, R.H, R.L);
		printf(" SP: %04X", R.SP);
		printf(" PC: 00:%04X", R.PC);
		printf(" (%02X %02X %02X %02X)", mem.read(R.PC), mem.read(R.PC + 1), mem.read(R.PC + 2), mem.read(R.PC + 3));
		printf("\n");
	}
}

void gameboy::run_until_breakpoint() {
	while (!ppu.is_ready_to_render() &&
		   debug.get_debug_state() == Debug_State::BREAKPOINT) {
		run_one_cycle();
		debug.handle_breakpoints();
	}
}

void gameboy::run_for_x_cycles(int cycles_to_run) {
	while (cycles_to_run > 0) {
		int cycles = run_one_cycle(false);
		if (cycles == -1)
			return;
		cycles_to_run -= cycles;
	}
}
int gameboy::run_one_cycle(bool skip_render) {
#if 0
	auto aa = cpu.R;
	log_cpu(aa);
#endif
	cpu.handle_interrupts();
	const int last_cycle = cpu.update();
	if (in_test_mode && cpu.op_code == 0x40)
		return -1;
	ppu.update(last_cycle, skip_render);
	cpu.handle_timer(last_cycle);
	apu.update(last_cycle);
	return last_cycle;
}

void gameboy::run_until_vblank() {
	bool vblank = false;
	s32 cycles_sum = 0;
	while (!vblank && debug.get_debug_state() == Debug_State::RUN && cycles_sum <= 70224) {
		cpu.handle_interrupts();
		const int last_cycle = cpu.update();
		vblank = ppu.update(last_cycle, false);
		cpu.handle_timer(last_cycle);
		cycles_sum += last_cycle;
		apu.update(last_cycle);
	}
}
