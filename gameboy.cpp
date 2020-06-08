#include "gameboy.h"


gameboy::gameboy() {
}

gameboy::~gameboy() {
}


void gameboy::startemu() {
	//ppu.createWindow();
	//ppu.renderScreen();
	rom.loadROM("./testroms/cpu_instrs/cpu_instrs.gb");

	cpu.update();
	cpu.update();
	cpu.update();
	cpu.update();
	cpu.update();
	cpu.update();
	cpu.update();
	cpu.update();
	cpu.update();
	cpu.update();

	cpu.setFlag(Flag::n);
	mem.write(0x1234, 0x33);
}

