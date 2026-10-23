#include <chrono>
#include <thread>

#include "Renderer.hpp"
#include "Audio.hpp"
#include "gameboy.hpp"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
// #include "Audio.hpp"

#include "utl/log.hpp"
#include "utl/type.hpp"

constexpr double frameTimeMicro = (1.0 / 59.73) * 1e6;

void render(std::string *file, Renderer *renderer) {
	renderer->start_new_imgui_frame();
	renderer->main_view(file);
	renderer->update_views();
	renderer->render();
}

int main(int argc, char *args[]) {
	init_logging(".");

	gameboy gb;
	std::string file = "";

	file = "C:/Users/knaub/source/repos/testroms/sml.gb";
	bool rom_loaded = false;

	const f64 target_FPS = 59.73;
	const f64 target_duration = 1 / target_FPS;
	f64 measured_duration = 0.0;

	Renderer renderer(&gb);
	Audio audio(&gb);

	audio.win32_init_dsound(glfwGetWin32Window(renderer.window));

	i32 Cycle_this_frame = 0;
	while (!glfwWindowShouldClose(renderer.window)) {
		glfwPollEvents();
		// renderer.start_new_imgui_frame();
		// renderer.main_view(&file);
		auto samps = gb.apu.apu_sample_count;

		if (rom_loaded) {
			while (Cycle_this_frame < CYCLES_PER_FRAME) {
				const i32 Last_cycle = gb.run_one_cycle();
				Cycle_this_frame += Last_cycle;
				if (gb.ppu.is_ready_to_render()) {
					// audio.update(Last_cycle);
					render(&file, &renderer);
					samps = gb.apu.apu_sample_count;
					break;
				}
			}
			Cycle_this_frame -= CYCLES_PER_FRAME;
		}
		if (file[0] != 0) {
			gb.card.create_sav_file();
			gb.load_rom(file, gb.skip_boot_rom);
			gb.card.load_sav_file();
			file.clear();
			rom_loaded = true;
		}
		render(&file, &renderer);
	}
	gb.card.create_sav_file();
	return 0;
}
