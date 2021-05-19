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
#include "spdlog/spdlog.h"
#include <fmt/core.h>
#include "docopt/docopt.h"

#include <iostream>

static const char USAGE[] =
	R"(Gameboy Emulator

    Usage:
	  cpp-gb-w -r <path_from_argument>
	  cpp-gb-w (-s | --skipboot) -f
	  cpp-gb-w (-s | --skipboot)
	  cpp-gb-w (-h | --help)
	  cpp-gb-w --version
	  cpp-gb-w -f
	  cpp-gb-w -a
	  cpp-gb-w 

    Options:
	  -r				Run chosen Rom at startup.
	  -f				Don't throttle the FPS
      -s --skipboot     Skip the boot animation.
      -h --help     	Show this screen.
      --version     	Show version.
)";

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) {

	gameboy gb;
	std::string file = "";

	file = "C:/Users/knaub/source/repos/testroms/dmg-acid2.gb";
	bool rom_loaded = false;

	bool skip_boot_rom = true;
	bool fps_drossel = false;

	std::map<std::string, docopt::value>
		args = docopt::docopt(USAGE,
							  {argv + 1, argv + argc},
							  true,			// show help if requested
							  "pre-alpha"); // version string

	for (auto const &arg : args) {
		std::cout << arg.first << " " << arg.second << std::endl;
		if (arg.first == "--skipboot" && arg.second.asBool() == true)
			skip_boot_rom = true;
		if (arg.first == "-f" && arg.second.asBool() == true)
			fps_drossel = false;
		if (arg.first == "-a" && arg.second.asBool() == true) {
			file = "C:/Users/knaub/source/repos/testroms/dmg-acid2.gb";
		}
	}

	const double target_FPS = 60;
	const double target_duration = 1 / target_FPS;
	double measured_duration = 0.0;

	// gb.load_rom("C:/Users/knaub/source/repos/testroms/cpu_instrs/individual/02-interrupts.gb", true);

	Renderer renderer(&gb.joypad);

	gb.debug.setDebugState(Debug_State::RUN);

	while (!glfwWindowShouldClose(renderer.window)) {

		double t_start = glfwGetTime();
		double d_time = t_start - measured_duration;

		if (d_time >= target_duration || !fps_drossel) {
			measured_duration = t_start;

			glfwPollEvents();
			renderer.start_new_imgui_frame();
			renderer.main_view(&gb.cpu, &gb.ppu, &gb.mem, &gb.card, &gb.joypad, &file);

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
			if (file[0] != 0) {
				gb.card.create_sav_file();
				gb.load_rom(file, skip_boot_rom);
				gb.card.load_sav_file();
				file.clear();
				rom_loaded = true;
			}

			renderer.render();
		}
	}
	gb.card.create_sav_file();
	return 0;
}
