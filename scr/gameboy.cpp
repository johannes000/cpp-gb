#include "gameboy.hpp"

#include "Debugger/Debugger.hpp"
#include "utl/type.hpp"

gameboy::gameboy()
	: mem_(&cpu_, &card_, &ppu_, &joypad_, &apu_),
	  cpu_(&mem_),
	  ppu_(&mem_, &cpu_),
	  joypad_(&mem_),
	  apu_(&mem_),

	  in_test_mode_(false),
	  skip_boot_rom_(false),
	  fps_drossel_(true) {
	reset(skip_boot_rom_);
}

auto gameboy::load_rom(std::string rom_file, bool skip_boot) -> bool {
	reset(skip_boot);

	if (rom_file.empty())
		return false;
	card_.load_rom(rom_file, skip_boot);

	return true;
}

auto gameboy::reset(bool skip_boot) -> void {
	mem_.reset(skip_boot);
	cpu_.init_register(skip_boot);
	ppu_.reset();
}

auto gameboy::run_one_cycle() -> int {
	return run_one_cycle(false);
}

auto gameboy::log_cpu(CPU_Register R) -> void {
	if (!cpu_.is_in_boot()) {
		printf("A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X", R.A_, R.F_, R.B_, R.C_, R.D_, R.E_, R.H_, R.L_);
		printf(" SP: %04X", R.SP_);
		printf(" PC: 00:%04X", R.PC_);
		printf(" (%02X %02X %02X %02X)", mem_.read(R.PC_), mem_.read(R.PC_ + 1), mem_.read(R.PC_ + 2), mem_.read(R.PC_ + 3));
		printf("\n");
	}
}

auto gameboy::run_for_x_cycles(int cycles_to_run) -> void {
	while (cycles_to_run > 0) {
		int cycles = run_one_cycle(false);
		if (cycles == -1)
			return;
		cycles_to_run -= cycles;
	}
}
auto gameboy::run_one_cycle(bool skip_render) -> int {
#if 0
	auto aa = cpu.R;
	log_cpu(aa);
#endif
	cpu_.handle_interrupts();
	const int last_cycle = cpu_.update();
	if (in_test_mode_ && cpu_.op_code_ == 0x40)
		return -1;
	ppu_.update(last_cycle, skip_render);
	cpu_.handle_timer(last_cycle);
	apu_.update(last_cycle);
	return last_cycle;
}

auto gameboy::run_until_vblank() -> void {
	bool vblank = false;
	s32 cycles_sum = 0;
	while (!vblank && cycles_sum <= CYCLES_PER_FRAME) {
		cpu_.handle_interrupts();
		const int last_cycle = cpu_.update();
		vblank = ppu_.update(last_cycle, false);
		cpu_.handle_timer(last_cycle);
		cycles_sum += last_cycle;
		apu_.update(last_cycle);
	}
}
