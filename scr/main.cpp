#include "gameboy.h"
#include "GUI/Gui.hpp"
#include "utl/type.h"

int main(int argc, char **argv) {
	loguru::init(argc, argv);

	loguru::add_file("logs/everything.log", loguru::Append, loguru::Verbosity_MAX);
	loguru::add_file("logs/error.log", loguru::Truncate, loguru::Verbosity_ERROR);
	bool done = false;
	Gui gui;
	gameboy gb;

	gb.load_rom("roms/sml.gb", true);
	i32 Cycle_this_frame = 0;
	while (!done) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(gui.getContext()->window))
				done = true;
		}

		while (Cycle_this_frame < CYCLES_PER_FRAME) {
			const i32 Last_cycle = gb.run_one_cycle();
			Cycle_this_frame += Last_cycle;
			if (gb.ppu.is_ready_to_render()) {
				// audio.update(Last_cycle);
				gui.render(gb);
				break;
			}
		}
		Cycle_this_frame -= CYCLES_PER_FRAME;
		// gui.render(gb);
	}
	gui.destroy();

	return 0;
}
