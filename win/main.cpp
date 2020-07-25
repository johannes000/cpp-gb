// cpp-gb.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet
// die Ausführung des Programms.
//

#include "Renderer.h"
#include "gameboy.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* args[]) {
	gameboy gb;

	Renderer renderer(&gb.joypad);

	gb.load_rom(renderer.file_browser().c_str());

	while (!glfwWindowShouldClose(renderer.window)) {
		gb.run_until_vblank();
		glfwPollEvents();
		renderer.update_views(&gb.ppu, &gb.debug);
	}
	return 0;
}
