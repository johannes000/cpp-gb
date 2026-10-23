#include "Audio.hpp"
#include "gameboy.hpp"
#include "GUI/Gui.hpp"
#include "utl/log.hpp"
#include "utl/type.hpp"
#include <filesystem>

auto main(int argc, char **argv) -> int {
	init_logging();

	bool done = false;
	Gui gui;
	Audio audio;
	gameboy gb;

	gb.load_rom("roms/mem_timing/mem_timing.gb", false);
	i32 Cycle_this_frame = 0;
	while (!done) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(gui.getContext()->window_))
				done = true;
		}

		while (Cycle_this_frame < CYCLES_PER_FRAME) {
			const i32 Last_cycle = gb.run_one_cycle();
			Cycle_this_frame += Last_cycle;
			if (gb.ppu_.is_ready_to_render()) {
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
