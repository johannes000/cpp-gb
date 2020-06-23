#include "gameboy.h"

gameboy::gameboy() : mem(&cpu, &card, &ppu), cpu(&mem), ppu(&mem) {}

gameboy::~gameboy() {

}

void gameboy::start_emu() {
    auto cycles = 0;
    auto last_cycle = 0;
    mem.reset();
    card.load_rom("./../testroms/tetris.gb");
    // cpu.print_all_ops();
    bool quit = false;
    while (!quit) {
        last_cycle = cpu.update();
        cycles += last_cycle;
        ppu.update(last_cycle * 4);
        step_mode = false;
        if (step_mode) {
            ppu.next_step();
        }
        // SDL_Delay(20);
    }
}
