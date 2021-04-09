
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

    glfwPollEvents();

    renderer.start_new_imgui_frame();
    renderer.update_views(&gb.ppu, &gb.debug);

    file = renderer.rom_select_view();
    if (!file.empty()) gb.load_rom(file.c_str());

    renderer.render();
  }
  return 0;
}
