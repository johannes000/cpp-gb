// cpp-gb.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet
// die Ausführung des Programms.
//

#include "Renderer.h"
#include "gameboy.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *args[]) {
    gameboy gb;

    Renderer renderer(&gb.joypad);

    gb.mem.reset();

    std::string file = renderer.file_browser();
    if (file.empty()) exit(0);
    gb.card.load_rom(file.c_str());

    while (!glfwWindowShouldClose(renderer.window)) {
        while (!gb.ppu.is_ready_to_render()) {
            const int last_cycle = gb.cpu.update();
            gb.cpu.handle_interrupts();
            gb.cpu.handle_timer(last_cycle);
            gb.ppu.update(last_cycle * 4);
        }
        glfwPollEvents();
        renderer.update_views(&gb.ppu, &gb.debug);
    }
    return 0;
}
