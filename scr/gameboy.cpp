#include "gameboy.h"


gameboy::gameboy() {
}

gameboy::~gameboy() {
}


void gameboy::startemu() {
	ppu.createWindow();
	ppu.renderScreen();
	rom.loadROM("./testroms/cpu_instrs/cpu_instrs.gb");
	//cpu.printallops();
	for (size_t i = 0; i < 100; i++) {
		cpu.update();
		ppu.waitforinput();
		SDL_Delay(200);
	}
	ppu.closeWindow();
}

