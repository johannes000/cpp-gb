#include "gameboy.h"


gameboy::gameboy() {

}

gameboy::~gameboy() {
}


void gameboy::startemu() {
	mem.connect(&cpu, &card, &ppu);
	ppu.createWindow();
	ppu.renderScreen();
	card.loadROM("./testroms/cpu_instrs/cpu_instrs.gb");
	// cpu.printallops();
	for (size_t i = 0; i < 255; i++) {
		cpu.update();
		ppu.waitforinput();
		SDL_Delay(200);
	}
	ppu.closeWindow();
}

