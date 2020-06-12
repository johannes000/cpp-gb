#include "gameboy.h"


gameboy::gameboy() :cpu(&mem), mem(&cpu, &card, &ppu), ppu() {

}

gameboy::~gameboy() {
}


void gameboy::startemu() {
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

