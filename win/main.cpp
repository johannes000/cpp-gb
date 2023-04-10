#include <chrono>
#include <thread>

#include "Renderer.h"
#include "gameboy.h"

// #include "Audio.h"

#include "utl/type.h"

constexpr double frameTimeMicro = (1.0 / 59.73) * 1e6;

void render(std::string *file, Renderer *renderer) {
	renderer->start_new_imgui_frame();
	renderer->main_view(file);
	renderer->update_views();
	renderer->render();
}

int main(int argc, char *args[]) {
	loguru::init(argc, args);

	loguru::add_file("everything.log", loguru::Append, loguru::Verbosity_MAX);
	loguru::add_file("error.log", loguru::Append, loguru::Verbosity_ERROR);

	gameboy gb;
	std::string file = "";

	file = "C:/Users/knaub/source/repos/testroms/sml.gb";
	bool rom_loaded = false;

	const f64 target_FPS = 59.73;
	const f64 target_duration = 1 / target_FPS;
	f64 measured_duration = 0.0;

	auto Frame_Start = std::chrono::system_clock::now();
	auto Frame_End = std::chrono::system_clock::now();

	Renderer renderer(&gb);

	i32 Cycle_this_frame = 0;
	while (!glfwWindowShouldClose(renderer.window)) {

		f64 t_start = glfwGetTime();
		f64 d_time = t_start - measured_duration;

		measured_duration = t_start;

		glfwPollEvents();
		// renderer.start_new_imgui_frame();
		// renderer.main_view(&file);
		auto samps = gb.apu.apu_sample_count;

		if (rom_loaded) {
			while (Cycle_this_frame < CYCLES_PER_FRAME) {
				i32 Last_cycle = gb.run_one_cycle();
				Cycle_this_frame += Last_cycle;
				if (gb.ppu.is_ready_to_render()) {
					// audio.update(22.2);
					// render(&file, &renderer);
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

		Frame_Start = std::chrono::system_clock::now();
		std::chrono::duration<double, std::micro> usedFrameTime = Frame_Start - Frame_End;
		if (usedFrameTime.count() < frameTimeMicro) {
			std::chrono::duration<double, std::micro> remainingFrameTime(frameTimeMicro - usedFrameTime.count());
			auto remainingFrameTimeLong = std::chrono::duration_cast<std::chrono::microseconds>(remainingFrameTime);
			std::this_thread::sleep_for(std::chrono::microseconds(remainingFrameTimeLong.count()));
		}
		Frame_End = std::chrono::system_clock::now();
	}
	gb.card.create_sav_file();
	return 0;
}
