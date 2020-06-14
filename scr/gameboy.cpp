#include "gameboy.h"


gameboy::gameboy() :cpu(&mem), mem(&cpu, &card, &ppu), ppu(&mem) {}

gameboy::~gameboy() {}


void gameboy::startemu() {
	card.loadROM("./testroms/cpu_instrs/cpu_instrs.gb");
	// cpu.printallops();
	for (size_t i = 0; i < 255; i++) {
		cpu.update();
		ppu.waitforinput();
		ppu.update();
		SDL_Delay(200);
	}

}

