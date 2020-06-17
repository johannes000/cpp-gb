#include "gameboy.h"
#include <time.h>
#include <chrono>
#include <thread>

gameboy::gameboy() :cpu(&mem), mem(&cpu, &card, &ppu), ppu(&mem) {}

gameboy::~gameboy() {
}


void gameboy::startemu() {
	int cycles = 0;
	card.loadROM("./testroms/tetris.gb");
	// cpu.printallops();

	while (true) {
		cycles += cpu.update();
		std::cout << std::dec << cycles << std::endl;
		ppu.waitforinput();
		ppu.update(cycles);
		//SDL_Delay(20);
	}

}

