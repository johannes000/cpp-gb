/**
 * @file main.cpp
 * @author Johannes
 * @brief
 * @date 15-06-2020
 *
 * @copyright Copyright (c) 2020
 */

#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include "type.h"
#include "gameboy.cpp"

#include "Audio.h"
#include "Audio.cpp"

#include "Renderer.h"
#include "Renderer.cpp"

#include <SDL2\SDL.h>

// Für den Windows WindowHnd
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <docopt/docopt.h>
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
	  cpp-gb-w -e
	  cpp-gb-w -e -s -f
	  cpp-gb-w 

    Options:
	  -r				Run chosen Rom at startup.
	  -e				Boot without rom
	  -f				Don't throttle the FPS
      -s --skipboot     Skip the boot animation.
      -h --help     	Show this screen.
      --version     	Show version.
)";

#ifndef _DEBUG
spdlog::set_level(spdlog::level::err);
#endif // DEBUG

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *args[]) {

	gameboy gb;
	std::string file = "";

	file = "C:/Users/knaub/source/repos/testroms/sml.gb";
	bool rom_loaded = false;

	// std::map<std::string, docopt::value>
	// 	args = docopt::docopt(USAGE,
	// 						  {args + 1, args + argc},
	// 						  true,			// show help if requested
	// 						  "pre-alpha"); // version string

	// for (auto const &arg : args) {
	// 	if (arg.first == "--skipboot" && arg.second.asBool() == true)
	// 		gb.skip_boot_rom = true;
	// 	if (arg.first == "-f" && arg.second.asBool() == true)
	// 		gb.fps_drossel = false;
	// 	if (arg.first == "-a" && arg.second.asBool() == true) {
	// 		file = "C:/Users/knaub/source/repos/testroms/dmg-acid2.gb";
	// 	}
	// 	if (arg.first == "-e" && arg.second.asBool() == true) {
	// 		file = "";
	// 	}
	// }

	const f64 target_FPS = 60;
	const f64 target_duration = 1 / target_FPS;
	f64 measured_duration = 0.0;

	Audio audio(&gb);
	Renderer renderer(&gb, &audio);

	HWND hwnd = glfwGetWin32Window(renderer.window);
	audio.win32_init_dsound(hwnd);

	gb.debug.setDebugState(Debug_State::RUN);

	i32 Cycle_this_frame = 0;
	while (!glfwWindowShouldClose(renderer.window)) {

		f64 t_start = glfwGetTime();
		f64 d_time = t_start - measured_duration;

		if (d_time >= target_duration || !gb.fps_drossel) {
			measured_duration = t_start;

			glfwPollEvents();
			renderer.start_new_imgui_frame();
			renderer.main_view(&file);

			if (rom_loaded) {
#if 0 
				switch (gb.debug.get_debug_state()) {
					case Debug_State::RUN: {
						bool Sound_in_queue = SDL_GetQueuedAudioSize(audio.audio_device_id) > des_samples * sizeof(i16) * 2;
						while (!Sound_in_queue && !gb.ppu.is_ready_to_render() && gb.debug.get_debug_state() == Debug_State::RUN) {
							gb.run_one_cycle(false);
							auto sam = gb.apu.apu_sample_count;
							if (sam >= des_samples) {
								SDL_QueueAudio(audio.audio_device_id, gb.apu.get_output_sample(des_samples), des_samples * sizeof(i16) * 2);
							}
							Sound_in_queue = SDL_GetQueuedAudioSize(audio.audio_device_id) > des_samples * sizeof(i16) * 2;
						}
					} break;
					case Debug_State::STEP: {
						if (gb.debug.get_next_step()) {
							gb.run_one_cycle();
							gb.debug.handle_breakpoints();
						}
					} break;
					case Debug_State::BREAKPOINT: {
						gb.run_until_breakpoint();
					} break;
				}
#else
				while (Cycle_this_frame < CYCLES_PER_FRAME) {
					i32 Last_cycle = gb.run_one_cycle();
					Cycle_this_frame += Last_cycle;
					if (gb.ppu.is_ready_to_render()) {
						auto samps = gb.apu.apu_sample_count;
						SDL_QueueAudio(audio.audio_device_id, gb.apu.get_output_sample(samps), samps * 2);
						break;
					}
				}
				Cycle_this_frame -= CYCLES_PER_FRAME;
				renderer.update_views();

#endif
			}
			if (file[0] != 0) {
				gb.card.create_sav_file();
				gb.load_rom(file, gb.skip_boot_rom);
				gb.card.load_sav_file();
				file.clear();
				rom_loaded = true;
			}
			renderer.render();

			// audio.update(target_duration);
		}
	}
	gb.card.create_sav_file();
	return 0;
}
