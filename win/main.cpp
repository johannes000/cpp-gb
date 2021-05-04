/**
 * @file main.cpp
 * @author Johannes
 * @brief
 * @date 15-06-2020
 *
 * @copyright Copyright (c) 2020
 */

#include "Renderer.h"
#include "gameboy.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *args[]) {
	const double target_FPS = 60;
	const double target_duration = 1 / target_FPS;
	double measured_duration = 0.0;

	bool fps_drossel = true;
	bool skip_boot_rom = true;

	gameboy gb;
	// gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/02-interrupts.gb", true);
	// gb.run_for_x_cycles(600000);

	Renderer renderer(&gb.joypad);
	bool rom_loaded = false;

	const char *file;
	file = "";
	gb.debug.setDebugState(Debug_State::RUN);

	while (!glfwWindowShouldClose(renderer.window)) {

		double t_start = glfwGetTime();
		double d_time = t_start - measured_duration;

		if (d_time >= target_duration || !fps_drossel) {
			measured_duration = t_start;

			glfwPollEvents();
			renderer.start_new_imgui_frame();
			if (rom_loaded) {
				switch (gb.debug.get_debug_state()) {
					case Debug_State::RUN:
						gb.run_until_vblank();
						break;
					case Debug_State::STEP:
						if (gb.debug.get_next_step()) {
							gb.run_one_cycle();
							gb.debug.handle_breakpoints();
						}
						break;
					case Debug_State::BREAKPOINT:
						gb.run_until_breakpoint();
						break;
				}

				renderer.update_views(&gb.ppu, &gb.debug);
			}
			file = renderer.rom_select_view();
			if (file[0] != 0) {
				gb.load_rom(file, skip_boot_rom);
				rom_loaded = true;
			}

			renderer.render();
		}
	}
	return 0;
}
