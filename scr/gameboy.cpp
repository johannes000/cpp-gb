#include "gameboy.h"


gameboy::gameboy() :cpu(&mem), mem(&cpu, &card, &ppu), ppu() {}

gameboy::~gameboy() {}


void gameboy::startemu() {
	//card.loadROM("./testroms/cpu_instrs/cpu_instrs.gb");
	card.loadROM("./testroms/tetris.gb");
	// cpu.printallops();
	for (size_t i = 0; i < 0xFFFF; i++) {
		cpu.update();
		ppu.waitforinput();
		//SDL_Delay(100);
	}

}

