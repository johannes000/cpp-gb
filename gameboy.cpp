#include "gameboy.h"


gameboy::gameboy() {
}

gameboy::~gameboy() {
}


void gameboy::startemu() {
	//ppu.createWindow();
	//ppu.renderScreen();
	rom.loadROM("./testroms/cpu_instrs/cpu_instrs.gb");
	for (size_t i = 0; i < 30; i++) {
		cpu.update();
	}


	cpu.setFlag(Flag::n);
	mem.write(0x1234, 0x33);
}

