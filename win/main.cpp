// cpp-gb.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet
// die Ausführung des Programms.
//

#include "Renderer.h"
#include "gameboy.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* args[]) {
	gameboy gb;

	Renderer renderer(&gb.joypad);

	std::string file;
	file = renderer.file_browser();
	gb.load_rom(file.c_str());
	file = "";

	while (!glfwWindowShouldClose(renderer.window)) {
		gb.run_until_vblank();
		glfwPollEvents();

		renderer.start_new_imgui_frame();
		renderer.update_views(&gb.ppu, &gb.debug);

		file = renderer.rom_select_view();
		if (!file.empty()) gb.load_rom(file.c_str());

		renderer.render();
	}
	return 0;
}
