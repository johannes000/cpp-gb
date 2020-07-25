// cpp-gb.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet
// die Ausführung des Programms.
//

#include "Renderer.h"
#include "gameboy.h"

void load_rom(gameboy* gb, Renderer* renderer);

int main([[maybe_unused]] int argc, [[maybe_unused]] char* args[]) {
	gameboy gb;

	Renderer renderer(&gb.joypad);

	load_rom(&gb, &renderer);

	while (!glfwWindowShouldClose(renderer.window)) {
		gb.run_until_vblank();
		glfwPollEvents();
		renderer.update_views(&gb.ppu, &gb.debug);
	}
	return 0;
}


void load_rom(gameboy* gb, Renderer* renderer) {
	gb->mem.reset();
	gb->cpu.init_register();
	gb->ppu.reset();

	std::string file = renderer->file_browser();
	if (file.empty()) exit(0);
	gb->card.load_rom(file.c_str());
}
