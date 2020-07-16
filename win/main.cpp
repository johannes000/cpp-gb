// cpp-gb.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet
// die Ausführung des Programms.
//

#include "Renderer.h"
#include "gameboy.h"


int main([[maybe_unused]] int argc, [[maybe_unused]] char *args[]) {
    int last_cycle;
    bool quit = false;
    SDL_Event event;
    gameboy gb;

    Renderer renderer;

    gb.mem.reset();
    std::string file = renderer.file_browser();
    gb.card.load_rom(file.c_str());

    while (!quit) {
        while (!gb.ppu.is_ready_to_render()) {
            last_cycle = gb.cpu.update();
            gb.cpu.handle_interrupts();
            gb.cpu.handle_timer(last_cycle);
            gb.ppu.update(last_cycle * 4);
        }
        renderer.update_views(&gb.ppu, &gb.debug);
        gb.joypad.process_sdl_events(&event);

    }
    return 0;
}
