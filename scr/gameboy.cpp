#include "gameboy.h"
#include <time.h>
#include <chrono>
#include <thread>

gameboy::gameboy() :cpu(&mem), mem(&cpu, &card, &ppu), ppu(&mem) {}

gameboy::~gameboy() {
}


void gameboy::startemu() {
	int c = 0;
	std::chrono::time_point<std::chrono::high_resolution_clock> current, previous;
	previous = std::chrono::high_resolution_clock::now();

	card.loadROM("./testroms/cpu_instrs/cpu_instrs.gb");
	// cpu.printallops();
	while (true) {
		current = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>> (current - previous);
		previous = current;

		cpu.update();
		ppu.waitforinput();
		if (!(++c % 100))
			ppu.update();
		//SDL_Delay(20);

		if (elapsed.count() < DELAY_TIME)
			std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(DELAY_TIME - elapsed.count()));

	}

}

