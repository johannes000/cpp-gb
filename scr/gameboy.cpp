#include "gameboy.h"
#include <time.h>

gameboy::gameboy() :cpu(&mem), mem(&cpu, &card, &ppu), ppu(&mem) {}

gameboy::~gameboy() {}


void gameboy::startemu() {
	int c = 0;
	card.loadROM("./testroms/cpu_instrs/cpu_instrs.gb");
	// cpu.printallops();
	while (true) {
		cpu.update();
		ppu.waitforinput();
		if (!(++c % 100))
			ppu.update();
		//SDL_Delay(20);
	}

}

